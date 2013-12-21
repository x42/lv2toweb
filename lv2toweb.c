/*
  Copyright 2007-2011 David Robillard <http://drobilla.net>
  Copyright 2013 Robin Gareus <robin@gareus.org>

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THIS SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#define _XOPEN_SOURCE 700

#ifndef VERSION
#define VERSION "0.0"
#endif

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>

#include "lv2/lv2plug.in/ns/ext/port-groups/port-groups.h"
#include "lv2/lv2plug.in/ns/ext/presets/presets.h"
#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/event/event.h"

#include <lilv/lilv.h>

#include "xhtmlcss.h"

#ifdef _MSC_VER
#    define isnan _isnan
#endif

LilvNode* control_class       = NULL;
LilvNode* event_class         = NULL;
LilvNode* group_pred          = NULL;
LilvNode* label_pred          = NULL;
LilvNode* preset_class        = NULL;
LilvNode* designation_pred    = NULL;
LilvNode* supports_event_pred = NULL;
LilvNode* uri_rdfs_comment    = NULL;
LilvNode* uri_atom_buffertype = NULL;
LilvNode* uri_atom_supports   = NULL;
LilvNode* uri_midi_event      = NULL;

static char *opt_screenshot   = NULL;
static char *opt_index        = NULL;
static char *opt_indextable   = NULL;
static int   opt_idx_author   = 1;

static const int port_direction(const LilvPlugin* p, uint32_t index) {
	const LilvPort* port = lilv_plugin_get_port_by_index(p, index);
	int direction = -1;
	if (!port) { return -1; }

	const LilvNodes* classes = lilv_port_get_classes(p, port);
	LILV_FOREACH(nodes, i, classes) {
		const LilvNode* value = lilv_nodes_get(classes, i);
		if (!strcmp(lilv_node_as_uri(value), "http://lv2plug.in/ns/lv2core#InputPort")) {
			direction=1;
		}
		else if (!strcmp(lilv_node_as_uri(value), "http://lv2plug.in/ns/lv2core#OutputPort")) {
			direction=2;
		}
	}
	return direction;
}

static const int port_type(const LilvPlugin* p, const LilvPort* port) {
	if (!port) { return -1; }
	const LilvNodes* classes = lilv_port_get_classes(p, port);
	LILV_FOREACH(nodes, i, classes) {
		const LilvNode* value = lilv_nodes_get(classes, i);
		if (!strcmp(lilv_node_as_uri(value), "http://lv2plug.in/ns/lv2core#ControlPort"))
			return 1;
		else if (!strcmp(lilv_node_as_uri(value), "http://lv2plug.in/ns/lv2core#AudioPort"))
			return 2;
		else if (!strcmp(lilv_node_as_uri(value), "http://lv2plug.in/ns/ext/atom#AtomPort")) {
			LilvNodes* atom_supports = lilv_port_get_value(p, port, uri_atom_supports);
			if (lilv_nodes_contains(atom_supports, uri_midi_event)) {
				lilv_nodes_free(atom_supports);
				return 4;
			}
			lilv_nodes_free(atom_supports);
			return 3;
		}
	}
	return 0;
}

static const char* port_class(const LilvPlugin* p, const LilvPort* port) {
	switch (port_type(p, port)) {
		case 1: return "controlport";
		case 2: return "audioport";
		case 3: return "atomport";
		case 4: return "midiport";
		default: return "";
	}
}

static const char* port_symbol(const LilvPlugin* p, const LilvPort* port) {
	if (!port) { return ""; }
	const LilvNode* sym = lilv_port_get_symbol(p, port);
	return lilv_node_as_string(sym);
}

static const char* port_name(const LilvPlugin* p, const LilvPort* port) {
	if (!port) { return ""; }
	LilvNode* name = lilv_port_get_name(p, port);
	return lilv_node_as_string(name);
}

static void print_lv2uri(const char* uri) {
	if        (!strncmp(uri, "http://lv2plug.in/ns/ext/atom#", 30)) {
		printf("atom:%s", &uri[30]);
	} else if (!strncmp(uri, "http://lv2plug.in/ns/lv2core#", 29)) {
		printf("lv2core:%s", &uri[29]);
	} else if (!strncmp(uri, "http://lv2plug.in/ns/ext/urid#", 30)) {
		printf("lv2urid:%s", &uri[30]);
	} else if (!strncmp(uri, "http://lv2plug.in/ns/extensions/ui#", 35)) {
		printf("lv2ui:%s", &uri[35]);
	} else if (!strncmp(uri, "http://lv2plug.in/ns/extensions/units#", 38)) {
		printf("lv2unit:%s", &uri[38]);
	} else if (!strncmp(uri, "http://lv2plug.in/ns/extensions/", 32)) {
		printf("lv2ext:%s", &uri[32]);
	} else if (!strncmp(uri, "http://lv2plug.in/ns/", 21)) {
		printf("lv2plugin:%s", &uri[21]);
	} else {
		printf("%s", uri);
	}
}

static const char* plugin_name(const LilvPlugin* p) {
	LilvNode* val = NULL;
	char const *title = "";
	val = lilv_plugin_get_name(p);
	if (val) {
		title = lilv_node_as_string(val);
		lilv_node_free(val);
	}
	return title;
}

static const char* plugin_class(const LilvPlugin* p) {
	const LilvPluginClass* pclass      = lilv_plugin_get_class(p);
	const LilvNode*       class_label  = lilv_plugin_class_get_label(pclass);
	if (class_label) {
		return lilv_node_as_string(class_label);
	}
	return "";
}

static const char * port_docs(const LilvPlugin* p, uint32_t idx) {
	LilvNodes* comments = lilv_port_get_value(p,
			lilv_plugin_get_port_by_index(p, idx),
			uri_rdfs_comment);
	if (comments) {
		const char *docs = lilv_node_as_string(lilv_nodes_get_first(comments));
		lilv_nodes_free(comments);
		return docs;
	}
	return NULL;
}

static const char * plugin_docs(const LilvPlugin* p) {
	LilvNodes* comments = lilv_plugin_get_value(p, uri_rdfs_comment);
	if (comments) {
		const char *docs = lilv_node_as_string(lilv_nodes_get_first(comments));
		lilv_nodes_free(comments);
		return docs;
	}
	return NULL;
}

static void count_ports(const LilvPlugin* p,
		int *in_audio, int *in_midi, int *in_ctrl,
		int *out_audio, int *out_midi, int *out_ctrl) {

	*in_audio = *in_midi = *in_ctrl = 0;
	*out_audio = *out_midi = *out_ctrl = 0;

	const uint32_t num_ports = lilv_plugin_get_num_ports(p);
	for (uint32_t i = 0; i < num_ports; ++i) {
		const LilvPort* port = lilv_plugin_get_port_by_index(p, i);
		if (!port) { continue; }
		if (port_direction(p, i) == 1) {
			switch (port_type(p, port)) {
				case 2: (*in_audio)++; break;
				case 4: (*in_midi)++; break;
				case 1: (*in_ctrl)++; break;
				case 3: (*in_ctrl)++; break;
				default: break;
			}
		} else if (port_direction(p, i) == 2) {
			switch (port_type(p, port)) {
				case 2: (*out_audio)++; break;
				case 4: (*out_midi)++; break;
				case 1: (*out_ctrl)++; break;
				case 3: (*out_ctrl)++; break;
				default: break;
			}
		}
	}
}

static void print_footer_with_mtime() {
	char timestring[200];
	time_t t;
	struct tm *tmp;
	t = time(NULL);
	tmp = gmtime(&t);
	strftime(timestring, sizeof(timestring), "Generated on %F %T %Z", tmp);
	printf(xhtml_footer, timestring);
}

struct kv {
	char *key;
	char *val;
};

static int cmpkvkey(const void *p1, const void *p2) {
	struct kv * const c1 = (struct kv * const) p1;
	struct kv * const c2 = (struct kv * const) p2;
#if 1
	const float v1 = atof(c1->key);
	const float v2 = atof(c2->key);
	if (v1 == v2) return 0;
	return v1 > v2;
#else
	return strcmp(c1->key, c2->key);
#endif
}

static void
print_port(const LilvPlugin* p,
           uint32_t          idx,
           float*            mins,
           float*            maxes,
           float*            defaults,
					 int               format
					 )
{
	const LilvPort* port = lilv_plugin_get_port_by_index(p, idx);
	if (!port) { return; }

	if (format) {
		printf("<div class=\"portinfo\" id=\"port%d\" style=\"display:none\">\n", idx);
	} else {
		printf("<div class=\"port\">\n");
	}

	const int direction = port_direction(p, idx);
	const int portType = port_type(p, port);
	int portAttrib=0;

	if (format) {
		printf("<h3>%d) %s</h3>\n", idx, port_name(p, port));
	} else {
		printf("<div class=\"toplink\"><a href=\"#top\">&nbsp;&uarr;&nbsp;</a></div>\n");
		printf("<h3><a name=\"portdoc%d\">%d</a>) %s</h3>\n", idx, idx, port_name(p, port));
	}
	printf("<p class=\"port %s\">(", port_class(p, port));

#if 0
	const LilvNode* sym = lilv_port_get_symbol(p, port);
	printf("<span>(%s)</span>\n", lilv_node_as_string(sym));
#endif

	switch(portType) {
		case 1:
			printf("<span>Control</span>");
			break;
		case 2:
			printf("<span>Audio</span>");
			break;
		case 3:
			printf("<span>Atom</span>");
			break;
		case 4:
			printf("<span>Midi</span>");
			break;
		default:
			break;
	}
	switch(direction) {
		case 1:
			printf(" <span>Input</span>");
			break;
		case 2:
			printf(" <span>Output</span>");
			break;
		default:
			break;
	}
	printf(")</p>\n");

	const char *doc = port_docs(p, idx);
	if (doc) {
		printf("<p class=\"docs\">%s</p>\n", doc);
	}

	LilvNodes* properties = lilv_port_get_properties(p, port);
	if (lilv_nodes_size(properties) > 0) {
		printf("<p class=\"desc\">Port Propert%s:</p>\n", lilv_nodes_size(properties) > 1 ? "ies": "y");
		printf("<ul>\n");
		LILV_FOREACH(nodes, i, properties) {
			const char *lu = lilv_node_as_uri(lilv_nodes_get(properties, i));
			printf("<li class=\"portproperty\">");
			if (!strcmp(lu, "http://lv2plug.in/ns/ext/port-props#notOnGUI")) {
				printf("Not On GUI");
			} else if (!strcmp(lu, "http://lv2plug.in/ns/lv2core#integer")) {
				printf("Integer");
				portAttrib|=1;
			} else if (!strcmp(lu, "http://lv2plug.in/ns/lv2core#reportsLatency")) {
				printf("Reports Latency\n");
			} else if (!strcmp(lu, "http://lv2plug.in/ns/dev/extportinfo#logarithmic")) {
				printf("Logarithmic\n");
			} else if (!strcmp(lu, "http://lv2plug.in/ns/lv2core#toggled")) {
				printf("Toggle\n");
			} else if (!strcmp(lu, "http://lv2plug.in/ns/lv2core#enumeration")) {
				printf("Enumeration\n");
				portAttrib|=2;
			} else {
				print_lv2uri(lu);
			}
			printf("</li>\n");
		}
		printf("</ul>\n");
	}
	lilv_nodes_free(properties);

	printf("<p>\n");

#if 0 // port classes
	LILV_FOREACH(nodes, i, classes) {
		const LilvNode* value = lilv_nodes_get(classes, i);
		printf("<span>%s</span>\n", lilv_node_as_uri(value));
	}
#endif
	
	if (lilv_port_is_a(p, port, event_class)) {
		LilvNodes* supported = lilv_port_get_value(
			p, port, supports_event_pred);
		if (lilv_nodes_size(supported) > 0) {
			LILV_FOREACH(nodes, i, supported) {
				const LilvNode* value = lilv_nodes_get(supported, i);
				printf("URI:<span>%s</span>\n", lilv_node_as_uri(value));
			}
		}
		lilv_nodes_free(supported);
	}
	printf("</p>\n");

#if 0 // TODO visually group ports
	LilvNodes* groups = lilv_port_get_value(p, port, group_pred);
	if (lilv_nodes_size(groups) > 0) {
		printf("Group: <span>%s</span>\n",lilv_node_as_string(lilv_nodes_get_first(groups)));
	}
	lilv_nodes_free(groups);
#endif

	LilvNodes* designations = lilv_port_get_value(p, port, designation_pred);
	if (lilv_nodes_size(designations) > 0) {
		printf("<p class=\"desc\">Designation%s:</p><ul>\n", lilv_nodes_size(designations) > 1 ? "s": "");
		LILV_FOREACH(nodes, i, designations) {
			const LilvNode* value = lilv_nodes_get(designations, i);
			const char *ds = lilv_node_as_string(value);
			printf("<li>");
			if (!strcmp(ds, "http://lv2plug.in/ns/ext/port-groups#left")) {
				printf("Port-group: Left");
			} else if (!strcmp(ds, "http://lv2plug.in/ns/ext/port-groups#right")) {
				printf("Port-group: Right");
			} else if (!strcmp(ds, "http://lv2plug.in/ns/lv2core#control")) {
				printf("Control-port");
			} else {
				print_lv2uri(ds);
			}
			printf("</li>\n");
		}
		printf("</ul>\n");
	}
	lilv_nodes_free(designations);


	if (format == 0 && lilv_port_is_a(p, port, control_class)) {
		// TODO print as integers if port is integer or enum
		if ((portAttrib&2) == 0) { // not enums
			printf("<table class=\"portrange\">\n");
				printf("<tr><th colspan=\"2\">Range</th></tr>\n");
			if (!isnan(mins[idx]))
				printf("<tr><th>min</th><td>%f</td></tr>\n", mins[idx]);
			if (!isnan(maxes[idx]))
				printf("<tr><th>max</th><td>%f</td></tr>\n", maxes[idx]);
			if (!isnan(defaults[idx]) && direction == 1)
				printf("<tr><th>default</th><td>%f</td></tr>\n", defaults[idx]);
			printf("</table>\n");
		} else if (!isnan(defaults[idx]) && direction == 1) {
			printf("<p><span class=\"desc\">Default:</span> %f</p>\n", defaults[idx]);
		}
	}

	LilvScalePoints* points = lilv_port_get_scale_points(p, port);
	if (format == 0 && lilv_scale_points_size(points) > 0) {
		struct kv *kvpairs;
		kvpairs = malloc(lilv_scale_points_size(points) * sizeof(struct kv));
		int j = 0;

		LILV_FOREACH(scale_points, i, points) {
			const LilvScalePoint* point = lilv_scale_points_get(points, i);
			kvpairs[j].key = strdup(lilv_node_as_string(lilv_scale_point_get_value(point)));
			kvpairs[j].val = strdup(lilv_node_as_string(lilv_scale_point_get_label(point)));
			j++;
		}
		qsort(&kvpairs[0], j, sizeof(struct kv), cmpkvkey);

		printf("<table class=\"portrange\">\n");
		printf("<tr><th colspan=\"2\">Scale Points</th></tr>\n");
		for (int i=0; i < j; ++i) {
			printf("<tr><th>%s</th><td>%s</td></tr>\n", kvpairs[i].key, kvpairs[i].val);
			free(kvpairs[i].key);
			free(kvpairs[i].val);
		}
		printf("</table>\n");
		free(kvpairs);
	}
	lilv_scale_points_free(points);

	printf("</div>\n");
}


static void print_plugin(LilvWorld* world, const LilvPlugin* p) {
	LilvNode* val = NULL;

	char const *title = plugin_name(p);

	/* HTML HEADER */
	printf(xhtml_header, title);

	const LilvPluginClass* pclass      = lilv_plugin_get_class(p);
	const LilvNode*        class_label = lilv_plugin_class_get_label(pclass);

	if (opt_index) {
		printf("<p class=\"backlink\"><a href=\"%s\">Back to Index</a></p>\n", opt_index);
	}

	printf("<h1 class=\"title\"><a name=\"top\"/>LV2 Doc &laquo;%s&raquo;\n", title);
	if (class_label) {
		printf(" [%s]\n", lilv_node_as_string(class_label));
	}
	printf("</h1>\n");

	printf("<div id=\"pluginmeta\">\n");

	const char *doc = plugin_docs(p);
	if (doc) {
		printf("<p class=\"desc\">Description</p>\n");
		printf("<p class=\"docs\">%s</p>\n", doc);
	}

	printf("<dl>\n");
	printf(" <dt>Title</dt><dd>%s</dd>\n", title);

	printf(" <dt>URI</dt><dd><a href=\"%s\">%s</a></dd>\n",
			lilv_node_as_uri(lilv_plugin_get_uri(p))
			,lilv_node_as_uri(lilv_plugin_get_uri(p))
			);

	if (class_label) {
		printf(" <dt>Class</dt><dd>%s</dd>\n", lilv_node_as_string(class_label));
	}

	val = lilv_plugin_get_author_name(p);
	if (val) {
		printf(" <dt>Author</dt><dd>%s</dd>\n", lilv_node_as_string(val));
		lilv_node_free(val);
	}

	val = lilv_plugin_get_author_email(p);
	if (val) {
		printf(" <dt>Author's Email</dt><dd>%s</dd>\n", lilv_node_as_uri(val));
		lilv_node_free(val);
	}

	val = lilv_plugin_get_author_homepage(p);
	if (val) {
		printf(" <dt>Author's Homepage</dt><dd><a href=\"%s\">%s</a></dd>\n",
				lilv_node_as_uri(val), lilv_node_as_uri(val));
		lilv_node_free(val);
	}

#if 0 // specifies port-number that reports latency
	if (lilv_plugin_has_latency(p)) {
		uint32_t latency_port = lilv_plugin_get_latency_port_index(p);
		printf(" <dt>Latency</dt><dd>%d</dd>\n", latency_port);
	}
#endif

#if 1 // List UIs
	LilvUIs* uis = lilv_plugin_get_uis(p);
	if (lilv_nodes_size(uis) > 0) {
		printf("<dt>User Interface%s</dt><dd><ul>\n", lilv_nodes_size(uis) > 1 ? "s" : "");
		LILV_FOREACH(uis, i, uis) {
			const LilvUI* ui = lilv_uis_get(uis, i);
			const LilvNodes* types = lilv_ui_get_classes(ui);
			LILV_FOREACH(nodes, t, types) {
				// TODO parse all UI types
				const char * pt = lilv_node_as_uri(lilv_nodes_get(types, t));
				printf("<li>");
				if (!strcmp(pt, "http://lv2plug.in/ns/extensions/ui#GtkUI")) {
					printf("Gtk+ UI");
				} else if (!strcmp(pt, "http://kxstudio.sf.net/ns/lv2ext/external-ui#Widget")) {
					printf("External UI (KXstudio)");
				} else if (!strcmp(pt, "http://lv2plug.in/ns/extensions/ui#external")) {
					printf("External UI (lv2plug.in)");
				} else if (!strcmp(pt, "http://lv2plug.in/ns/extensions/ui#Qt4UI")) {
					printf("Qt4 UI");
				} else if (!strcmp(pt, "http://lv2plug.in/ns/extensions/ui#WindowsUI")) {
					printf("Windows UI");
				} else if (!strcmp(pt, "http://lv2plug.in/ns/extensions/ui#Gtk3UI")) {
					printf("Gtk3 UI");
				} else if (!strcmp(pt, "http://lv2plug.in/ns/extensions/ui#CocoaUI")) {
					printf("CocoaUI UI (OSX)");
				} else if (!strcmp(pt, "http://lv2plug.in/ns/extensions/ui#X11UI")) {
					printf("X11 UI");
				} else {
					print_lv2uri(pt);
				}
				printf("</li>\n");
			}
		}
		printf("</ul></dd>\n");
	}
	lilv_uis_free(uis);
#endif

	/* Required Features */
	LilvNodes* features = lilv_plugin_get_required_features(p);
	if (lilv_nodes_size(features) > 0) {
		printf("<dt>Required Feature%s</dt><dd><ul>\n", lilv_nodes_size(features) > 1 ? "s" : "");
		LILV_FOREACH(nodes, i, features) {
			const char *ft = lilv_node_as_uri(lilv_nodes_get(features, i));
			printf("<li>");
			// TODO replace URI -> name
			if (!strcmp(ft, "http://lv2plug.in/ns/ext/urid#map")) {
				printf("URI map");
			} else if (!strcmp(ft, "http://lv2plug.in/ns/lv2core#isLive")) {
				printf("Live - must be run in realtime");
			} else {
				print_lv2uri(ft);
			}
			printf("</li>\n");


		}
		printf("</ul></dd>\n");
	}
	lilv_nodes_free(features);

	/* Optional Features */
	features = lilv_plugin_get_optional_features(p);
	if (lilv_nodes_size(features) > 0) {
		printf("<dt>Optional Feature%s</dt><dd><ul>\n", lilv_nodes_size(features) > 1 ? "s" : "");
		LILV_FOREACH(nodes, i, features) {
			const char *ft = lilv_node_as_uri(lilv_nodes_get(features, i));
			printf("<li>");
			// TODO replace URI -> name
			if (!strcmp(ft, "http://lv2plug.in/ns/extensions/ui#inPlaceBroken")) {
				printf("No In-Place Processing");
			} else if (!strcmp(ft, "http://lv2plug.in/ns/lv2core#hardRTCapable")) {
				printf("Hard Realtime Capable");
			} else if (!strcmp(ft, "http://lv2plug.in/ns/lv2core#isLive")) {
				printf("Live - can be run in realtime");
			} else {
				print_lv2uri(ft);
			}
			printf("</li>\n");
		}
		printf("</ul></dd>\n");
	}
	lilv_nodes_free(features);

	/* Extension Data */
	LilvNodes* data = lilv_plugin_get_extension_data(p);
	if (data && lilv_nodes_size(data) > 0) {
		printf("<dt>Extension Data</dt><dd><ul>\n");
		LILV_FOREACH(nodes, i, data) {
			const char *ed = lilv_node_as_uri(lilv_nodes_get(data, i));
			printf("<li>");
			if (!strcmp(ed, "http://lv2plug.in/ns/ext/state#interface")) {
				printf("State Interface");
			} else {
				print_lv2uri(ed);
			}
			printf("</li>\n");
		}
		printf("</ul></dd>\n");
	}
	lilv_nodes_free(data);

	LilvNodes* presets = lilv_plugin_get_related(p, preset_class);
	if (lilv_nodes_size(presets) > 0) {
		printf("<dt>Preset%s</dt><dd><ul>\n", lilv_nodes_size(presets) > 1 ? "s" : "");
		LILV_FOREACH(nodes, i, presets) {
			const LilvNode* preset = lilv_nodes_get(presets, i);
			lilv_world_load_resource(world, preset);
			LilvNodes* titles = lilv_world_find_nodes(
				world, preset, label_pred, NULL);
			if (titles) {
				const LilvNode* title = lilv_nodes_get_first(titles);
				printf("<li>%s</li>\n", lilv_node_as_string(title));
				lilv_nodes_free(titles);
			}
		}
		printf("</ul></dd>\n");
	}
	lilv_nodes_free(presets);

	printf("</dl>\n");

	if(opt_screenshot) {
		printf("<div class=\"screenshot\"><img alt=\"Screenshot\" class=\"screenshot\" src=\"%s\" onclick=\"showimg();\" /></div>\n", opt_screenshot);
	}
	printf("</div>\n"); // end meta box

	if(opt_screenshot) {
		printf("<div id=\"lightbox\" style=\"display:none;\" onclick=\"hideimg();\">\n");
		printf("<div><img alt=\"Screenshot\" src=\"%s\"/></div>\n", opt_screenshot);
		printf("</div>\n");
	}

	printf("<div id=\"portwrapper\">\n");
	/* Ports */
	const uint32_t num_ports = lilv_plugin_get_num_ports(p);
	float* mins     = (float*)calloc(num_ports, sizeof(float));
	float* maxes    = (float*)calloc(num_ports, sizeof(float));
	float* defaults = (float*)calloc(num_ports, sizeof(float));
	lilv_plugin_get_port_ranges_float(p, mins, maxes, defaults);

	/* index ports by direction */
	uint32_t p_cin, p_cout;
	uint32_t *p_xin, *p_xout;
	p_xin  = malloc(num_ports * sizeof(uint32_t));
	p_xout = malloc(num_ports * sizeof(uint32_t));
	p_cin = p_cout = 0;

	for (uint32_t i = 0; i < num_ports; ++i) {
		if (port_direction(p, i) == 1) p_xin[p_cin++] = i;
		else if (port_direction(p, i) == 2) p_xout[p_cout++] = i;
	}

	printf("<script type=\"text/javascript\">\n");
	printf("var num_ports=%d;\n", num_ports);
	printf("</script>\n");

	/* print port table */
	printf("<div id=\"plugintable\"><table class=\"plugintable\">\n");
	printf("<tr><td></td><td class=\"ptbl ptblTL\">&nbsp;</td><td class=\"ptbl ptblTC\">&nbsp;</td><td class=\"ptbl ptblTR\">&nbsp;</td><td></td></tr>\n");
	for (uint32_t i = 0; i < num_ports; ++i) {
		if (i >= p_cin && i >= p_cout) break;
		printf("<tr>");
		if (i < p_cin) {
			const LilvPort* port = lilv_plugin_get_port_by_index(p, p_xin[i]);
			printf("<td class=\"bgL %s\" onmouseover=\"showdoc(%d);\" onmouseout=\"showdoc(-1);\" onclick=\"jumpto(%d);\">%d) %s</td><td class=\"pbody left ptbl ptblLL\" onmouseover=\"showdoc(%d);\" onmouseout=\"showdoc(-1);\" onclick=\"jumpto(%d);\"><div>(%s)",
					port_class(p, port), p_xin[i], p_xin[i], p_xin[i], port_name(p, port),
					p_xin[i], p_xin[i], port_symbol(p,port));
			print_port(p, p_xin[i], mins, maxes, defaults, 1);
			printf("</div></td>");
		} else {
			printf("<td></td><td class=\"pbody ptbl ptblLL\"></td>");
		}
		printf("<td class=\"pbody\"><div>&nbsp;</div></td>");

		if (i < p_cout) {
			const LilvPort* port = lilv_plugin_get_port_by_index(p, p_xout[i]);
			printf("<td class=\"pbody right ptbl ptblRR\" onmouseover=\"showdoc(%d);\" onmouseout=\"showdoc(-1);\" onclick=\"jumpto(%d);\"><div>(%s)",
					p_xout[i], p_xout[i], port_symbol(p, port));
			print_port(p, p_xout[i], mins, maxes, defaults, 1);
			printf("</div></td><td class=\"bgR %s\" onmouseover=\"showdoc(%d);\" onmouseout=\"showdoc(-1);\" onclick=\"jumpto(%d);\">%d) %s</td>",
					port_class(p, port), p_xout[i], p_xout[i], p_xout[i], port_name(p,port));
		} else {
			printf("<td class=\"pbody ptbl ptblRR\"></td><td></td>");
		}
		printf("</tr>\n");
	}
	printf("<tr><td></td><td class=\"ptbl ptblBL\">&nbsp;</td><td class=\"ptbl ptblBC\">&nbsp;</td><td class=\"ptbl ptblBR\">&nbsp;</td><td></td></tr>\n");
	printf("</table></div>\n");

	printf("<div id=\"portlist\">\n");
	printf("<h2 style=\"text-align:center;\">Port List</h2>\n");

	printf("<div id=\"inputportlist\">\n");
	for (uint32_t i = 0; i < p_cin ; ++i) {
		print_port(p, p_xin[i], mins, maxes, defaults, 0);
	}
	printf("</div>\n");

	printf("<div id=\"outputportlist\">\n");
	for (uint32_t i = 0; i < p_cout ; ++i) {
		print_port(p, p_xout[i], mins, maxes, defaults, 0);
	}
	printf("</div>\n");

	printf("<div style=\"clear:both;\"></div>\n");
	printf("</div>\n"); // end portlist

	printf("</div>\n"); // end portwrapper

	free(mins);
	free(maxes);
	free(defaults);
	free(p_xin);
	free(p_xout);

	// HTML FOOTER
	print_footer_with_mtime();
}

static void usage (int status) {
	printf ("lv2toweb - create html documentation for LV2 plugins.\n\n");
	printf ("Usage: lv2toweb [ OPTIONS ] <plugin-uri>\n\n");
	printf ("Options:\n"
"\n");
	printf ("\n"
"  -A,                       do not include 'Author' in index table.\n"
"  -h, --help                display this help and exit\n"
"  -i, --index <path>        add link to index\n"
"  -s, --screenshot <path>   add link to screenshot\n"
"  -t, --tableindex <file>   create index page, read uris from file\n"
"  -V, --version             print version information and exit\n"
"\n");
	printf ("\n"
/*                                  longest help text w/80 chars per line ---->|\n" */
"Create a xhtml page with information about the given LV2 plugin.\n"
"A screenshot of the plugin can be included, the image needs to be created by\n"
"external means, the path is relative to the created page.\n"
"lv2toweb writes the page to standard-out.\n"
	"\n");
	printf ("Report bugs to Robin Gareus <robin@gareus.org>\n"
	        "Website: https://github.com/x42/lv2toweb/\n"
	        );
	exit (status);
}
	
static struct option const long_options[] =
{
	{"help", no_argument, 0, 'h'},
	{"index", required_argument, 0, 'i'},
	{"screeshot", required_argument, 0, 's'},
	{"tableindex", required_argument, 0, 't'},
	{"version", no_argument, 0, 'V'},
	{NULL, 0, NULL, 0}
};


static int decode_switches (int argc, char **argv) {
	int c;
	while ((c = getopt_long (argc, argv,
			   "A"	/* no-index-author */
			   "h"	/* help */
			   "i:"	/* index */
			   "s:"	/* screeshot */
			   "t:"	/* index table */
			   "V",	/* version */
			   long_options, (int *) 0)) != EOF)
	{ switch (c) {
	case 'A':
	  opt_idx_author = 0;
	  break;

	case 'i':
	  opt_index = optarg;
	  break;

	case 's':
	  opt_screenshot = optarg;
	  break;

	case 't':
	  opt_indextable = optarg;
	  break;

	case 'V':
	  printf ("lv2toweb version %s\n\n", VERSION);
	  printf ("Copyright (C) GPL 2013 Robin Gareus <robin@gareus.org>\n");
	  printf ("Copyright (C) GPL 2007-2011 David Robillard <http://drobilla.net>\n");
	  exit (0);

	case 'h':
	  usage (0);

	default:
	  usage (EXIT_FAILURE);
		}
	}
	return optind;
}

static void lv2world_init(LilvWorld* world) {
	control_class       = lilv_new_uri(world, LILV_URI_CONTROL_PORT);
	event_class         = lilv_new_uri(world, LILV_URI_EVENT_PORT);
	group_pred          = lilv_new_uri(world, LV2_PORT_GROUPS__group);
	label_pred          = lilv_new_uri(world, LILV_NS_RDFS "label");
	preset_class        = lilv_new_uri(world, LV2_PRESETS__Preset);
	designation_pred    = lilv_new_uri(world, LV2_CORE__designation);
	supports_event_pred = lilv_new_uri(world, LV2_EVENT__supportsEvent);
	uri_rdfs_comment    = lilv_new_uri(world, LILV_NS_RDFS "comment");
	uri_atom_buffertype = lilv_new_uri(world, LV2_ATOM__bufferType);
	uri_atom_supports   = lilv_new_uri(world, LV2_ATOM__supports);
	uri_midi_event      = lilv_new_uri(world, LILV_URI_MIDI_EVENT);
}

static void lv2world_free(LilvWorld* world) {
	lilv_node_free(supports_event_pred);
	lilv_node_free(designation_pred);
	lilv_node_free(preset_class);
	lilv_node_free(label_pred);
	lilv_node_free(group_pred);
	lilv_node_free(event_class);
	lilv_node_free(control_class);
	lilv_node_free(uri_rdfs_comment);
	lilv_node_free(uri_atom_buffertype);
	lilv_node_free(uri_atom_supports);
	lilv_node_free(uri_midi_event);
	lilv_world_free(world);
}

static int plugintohtml(LilvWorld* world, const char* plugin_uri) {
	LilvNode* uri = lilv_new_uri(world, plugin_uri);
	if (!uri) {
		fprintf(stderr, "Invalid plugin URI\n");
		return 1;
	}

	const LilvPlugins* plugins = lilv_world_get_all_plugins(world);
	const LilvPlugin*  p       = lilv_plugins_get_by_uri(plugins, uri);

	if (p) {
		print_plugin(world, p);
	} else {
		fprintf(stderr, "Plugin not found.\n");
	}

	lilv_node_free(uri);
	return (p != NULL ? 0 : -1);
}

static int indextable(LilvWorld* world, FILE *f) {

	const LilvPlugins* plugins = lilv_world_get_all_plugins(world);
	printf(xhtml_indexhead);
	printf("<table>\n");
	printf("<tr><th rowspan=\"2\" class=\"first\">Class</th><th rowspan=\"2\">Name</th>");
	printf("<th colspan=\"2\">Audio/Midi/CTRL</th>");
	printf("<th rowspan=\"2\">Description</th>\n");
	if (opt_idx_author) {
		printf("<th rowspan=\"2\">Author</th>\n");
	}
	printf("<th rowspan=\"2\">URI</th>\n");
	printf("</tr><tr>\n");
	printf("<th>in</th><th>out</th>");
	printf("</tr>\n");

	while (!feof(f)) {
		char furi[4096];
		if (!fgets(furi, 4096, f)) break;
		if (strlen(furi) == 0) break;
		furi[strlen(furi)-1] = '\0';

		LilvNode* uri = lilv_new_uri(world, furi);
		if (!uri) continue;
		const LilvPlugin* p = lilv_plugins_get_by_uri(plugins, uri);
		if (!p) continue;
		printf("<tr>");
		char *link = strdup(furi);
		for (char *tmp = link; *tmp; ++tmp) {
			if (*tmp >= 'a' && *tmp <= 'z') continue;
			if (*tmp >= '0' && *tmp <= '9') continue;
			if (*tmp >= 'A' && *tmp <= 'Z') continue;
			*tmp='_';
		}

		printf("<td class=\"first center\">%s</td>", plugin_class(p));
		printf("<td><a href=\"%s.html\">%s</a></td>", link, plugin_name(p));
		free(link);

		int in_audio, in_midi, in_ctrl;
		int out_audio, out_midi, out_ctrl;
		count_ports(p, &in_audio, &in_midi, &in_ctrl, &out_audio, &out_midi, &out_ctrl);
		printf("<td class=\"center\">%d/%d/%d</td>", in_audio, in_midi, in_ctrl);
		printf("<td class=\"center\">%d/%d/%d</td>", out_audio, out_midi, out_ctrl);

		const char *doc = plugin_docs(p);
		if (doc) {
			printf("<td>%s</td>", doc);
		} else {
			printf("<td class=\"center\">-</td>");
		}

		if (opt_idx_author) {
			LilvNode* nme = lilv_plugin_get_author_name(p);
			if (nme) {
				LilvNode *eml = lilv_plugin_get_author_email(p);
				if (eml) {
					printf("<td class=\"center\"><a href=\"%s?subject=LV2\" rel=\"nofollow\" class=\"nl\">%s</a></td>",
							lilv_node_as_string(eml),
							lilv_node_as_string(nme));
					lilv_node_free(eml);
				} else {
					printf("<td class=\"center\">%s</td>", lilv_node_as_string(nme));
				}
				lilv_node_free(nme);
			} else {
				printf("<td class=\"center\">?</td>");
			}
		}
		printf("<td><a href=\"%s\" rel=\"nofollow\" class=\"nl\">%s</a></td>", furi, furi);
		printf("</tr>\n");
		lilv_node_free(uri);
	}
	printf("<tr><td class=\"last\" colspan=\"7\"></td></tr>\n");
	printf("</table>\n");
	print_footer_with_mtime();
	return 0;
}

int main(int argc, char** argv) {
	int ret = 0;
	int opt = decode_switches (argc, argv);

	if (opt_indextable) {
		if (argc != opt) { usage(EXIT_FAILURE); }
	} else {
		if (argc != opt + 1) { usage(EXIT_FAILURE); }
	}

	LilvWorld* world = lilv_world_new();
	lilv_world_load_all(world);
	lv2world_init(world);

	if (!opt_indextable) {
		ret = plugintohtml(world, argv[opt]);
	} else {
		FILE *f;
		if (!strcmp(opt_indextable, "-")) {
			f=stdin;
		} else {
			f=fopen(opt_indextable, "r");
		}
		ret = indextable(world, f);
		fclose(f);
	}

	lv2world_free(world);
	return ret;
}
