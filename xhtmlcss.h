/*
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

// http://webcodertools.com/imagetobase64converter/Create
#define MIDI_ICON " background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAAAAXNSR0IArs4c6QAAAAZiS0dEAL8AvwC//ggzigAABa9JREFUSMfFlWtsFFUYht9zZmZnpjvdtrT0km13odwEUqCAULEoRAuIhQCCIHchhhiChB/QX2pEwjUxWOQmIgmkxIA2UKRcJBG5yEKhLYUIhF6goVJsu7ud3Z3ZnZ2LP2AHSPDyx3iSk3PynZnznO9955sD/MeNvCjo/IhBpNyAZVmiz+crvn0nOjEQDBRFY1ouAMo7uIfpKWnX+vTjTo4aNeocz/PBAV/2xZ2Vjf8OAAA+n2/GOd/v+w9VVzvDoSAy0jORkpIGAJC7ZXR0tkF0pmDm5Ml6yVjvmpycnPK8vDzjbwHCch6RcoU5fuTshfW7KopYUUJR4Si8NsIFl8sFlmUBAKZpQpZlXKgLWxevXiIxJYAViz5onlJaUOQp69Eh7zZfDLAsi/lqZ3Xd7oMVBcOGjsTsSf2QmZkJ03z8AiEElmXZI8Mw6OrqQtXZVpw9fwbL5ixoX7J4XKHL5WpP7MkCgLhchLpdxfGqMxd2Hdxf8Ma4ida8KYOJYRgwTROEEBBCoKoqKKXgeR6EEBiGgdTUVCyZmQGXJGFnxYHs9B5pV1paWvJ79+6tAwAFAHW7ipqamhlb9lYWDR5YiLmlg4hpmrAsyz5xPB7H5zuOY9Oen0ApfU5nXdfxzsR+GDO6GFv3fp0XDAbXJdboE2nEK7Wd+8OqhvfeHAbTNGGaJgzDwKnzD9HarIAQAn/nHwh2+8FxHPx+Py7VdNsQ0zQxoyQfDqeEkz/fK+vs7HTbgPr6+uJDp087CwYXItuT8phMKXy1YZTv2Ii9R09BkiRs/XQJNq6eA0IIdh66jI3b1uFSTTcIeWxlVlYWRhaMtCpPVaOpqWmy7cGDFv/EB/casWjSVFsSSilGFPAonToXg/r0gtPphCiKttlDXxoIXdcweAABwzB21iXD3eTEmQ48ehCeDmAPCwCdMi2CBeR64yBEAKUUhBCkpaXh4xVv26Y+87Xh/VkvY/60Yejq6oLD4YCu69B1HVK6A7yDtx4F9OFPTVbVXIGXwDAMACAWi2HNpu+wdPU2hEIh8DwPURQhCAIEQQDP8xAEAbXX/Zi/YiM27z5jZ8ayLJJEiURUVbIlYlhCQZ8WB6UUIu+E6lAhiiJ4ngfHcc8Zqus60pxRSE4XJKdgy/SYZNo1xgKAk3c+jKpKXjweB8MwSEpKwra1i8BxHJKTkyEIgp1dQiKGYTB6zDAcr9gATdMQiUTAMAx0XYfcHbRSpWTFlqhHRvgaYRncvmuC47jnKpVlWVBK7fmzPRFLZE0pRXMTQSyuksxM7aYN8Hg8J/v2G4RfGuosTdPsUyakSIzP9kQsUe2JwjzbUGflevsjKzuzygbk5uaee7dkgn6jvoY0NCgAAE3TEI/HoSgKFEVBNBpFJBKBqqqIRqNQFAWxWAyapsEwDFiWhZsNYdTVXyazJryF7OzsYzYgNTU1OKIwbc2QgiH49ugh+P1+GIYBTdMQjUYRCoUQDAYRCoUgyzJkWUY4HEYkErEhiqJg349H4c5yY3yx+7Db7W60AenreWRkZJSvWDC+OSKHsGHHCQQCAdu8UCiEcDiMlpYWtLa22hBFUexnNu8+jUcP27BqaUlAkqTFL/xdt7W19bz8a2PDZ9u/yRZECQunzMDQISJYloWiKFi1dh8klwuby+aCYRgYhoFbt+LYV/UDgv5OlC37UBn3eq+xHo+nNrEn8yygPPKFsmXeuorRQ3rN/u1uR0rlie9xvbnb0sISIWoS6psawTkcKMjxwFcfQMXJ89aR6kqSl5OHT1ZOC4wc0Xe81+ut5RayMK+bf31l3r9/nw0Gg+tqrnaVVV28gFs36qCEZYtSSihDocd1S3Klkvz+g1D6ajGKX8k+nJycvDg/P1/hFnCIH4j/8538BOT2+/2T29vbp/s7nMNDYUUyLUKSpSSlR4Z8s2fPnlVZWVnHvF5vI/6v9id7Nrq/MAzELwAAAABJRU5ErkJggg==)"

#define AUDIO_ICON "background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAAAAXNSR0IArs4c6QAAAAZiS0dEAAAAAAAA+UO7fwAAA1ZJREFUSMe1lk9MXFUUxn/3zpspA6VNKohmgKQxOmmRamyN0nahiXbiggqJEkhal5Ao6aJpUleksqpJV4omw8ZoSSBPExA2TnXZlh0bpHVMjQ1/Fq1/UoHyb+a9rwvvNPNotZji2dycl/O+75xzv3fOM5SZpC+Al4AngXXgpjHmNR7D7CbfAOazi18/BSSBWkk5R56V9Kmkj/8LgbfJjwH28pXLfHDy3STwBBCXNAUUgABYkzQOLAGLwF/GmA+3WsEOwLt+7TpAYnBkrM61qxHYmx0ebQGeA5qAF4GXgVcljUsakjQgqe/fCHYBFZ7nAcRm52YBKoFqYPfS0hKuqjogNTgydhB4tozwEHBI0icPEEi6CNQ6QAB7KXcJwMsOjyaAuO/7AAl3Pzvn5xcA9jjC+uzw6CtAGjgg6cv7BJKGgGc2EZgwDAHMxMREuW8/H/rGAnZ8/FuAxMBXfhVQ5eLqgL1As6TvraQfgP1Afd+FgQaXYUlRACwsLET6+OP0NIBxrbSTk5MAMRdX2XdhoBFoANLWXVoDsCeXy5WUFJVWLProytWrEX9mZgbAuDjP4ewCaixQMzgyVgNUFIvFh138AxaPx6Na9yJqNw4nPjgylrTAjnw+D2CNMWyHORyTz+eJPZ1+4SPf9zEVO83U1BRKVLJdp+/7j27HY5uk4PS585IUHnz9LUnSdp2nz52XBdbT6TRAKGm7kgZQOp3GA37v7mxLAknP86qA8FFKKhQKEd+p5j6+53kGKHR3tgUe8LP73GsymUyVm5gRgiAIIoBHDh+O+E1NTQAKgsAAxUwmE3eTdtUaY94ArgHz/Wd654CNUiYlgFQqFQF8vrkZQC7zsKWlBSBwcSv9Z3pngTkgb51uTwC/AL8BKyUCa/8eVa2treV++P6Jd0IgPH78bYCN3vc67gJ3Xdwt4Fdg2hjzplf2cZyU9J0bxwDhscwxCxR7utoF0NHRkXAVBkCxvj61G/gTWANWe7rafwJuu1V76mEbbRFYc6UHjQ2NnquoCITV1dW1wB9uX691d7bdAJbde4vAHeC2Mab/n1bmOlDct38fwEZ3Z9sd9+IyUOjpar/hsl3e6srcTBAA4dEjRwFWXbbzxpiMpKxrz4ox5uyW59L//dtyD6cqpLhqn3SnAAAAAElFTkSuQmCC);"

#define ATOM_ICON "background-image: url(data:image/png;base64,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);"

#define CONTROL_ICON "background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAAAAXNSR0IArs4c6QAAAAZiS0dEAL8AvwC//ggzigAAAphJREFUSMe1lcFKG1EUhr9MTE0WJTZhFEID3TQJmAxZSXHVuOgg6XvoVsjGZfYBN67yEq5mMc0iheJAwWKYVFBXXdi0sdgaW6qZxEw3Z8oQTWIUBy4DM/f8373n3PsfeOQnMOGfIiN4y1wXuAYGMtxpAAoQBqJATN4RAbkieAlcAGdAB7iS7xMBQRF8ASwCaSAJPANCMscBzoET4BA4AL4IsD8OEBShHFAAljRNy+i6Pp/L5WaTyaTiOA7tdnvQbDa7pmme2rZ9BHwE3gNN4JcfEhhKSxTIA2+B16VSabFYLM6OK6JhGN1KpXIgAANoyO4G3oo9UAR4CawCb5aXl/MrKyuheDw+9pSkUqmZbDar1mq1CNADvgug7wcEgTjwClgtlUpL6+vroc3NTdLpNJMgiURCUVU1ZllWXwAt4C/gKr70xIC0pmkZLy3VapVKpcLx8fHE814sFmc1TcsAGdEKesL+/Cd1XZ/3B04D0XVdBZ6LljIMCAPRXC53o6jVapW1tbWJAImNilbAD/DqEFpYWFBuC67X6xQKBRqNxkiAxIb8N1+Zxle2trbY2NiYyov8gGug1263B6Mm5/N56vX6SDGJ7fu9yQMMxEs6zWaze1/nlNhz8akbgA5wYprm6X0Bpmn+EH+68G6yH/ATOLRt+8gwjKl3YRhG17btQzG/M0n5/5vs1cAFnlqWNZfNZtVEInGnQ7C3t9cvl8ufgTrwAfgqtnEDcAl0gSe1Wi2iqmoslUrNTFp5uVz2zK4mO/jj1SA41KF6wG+pR8+yrP7+/n7Idd0n4XA4GI1GA47j0Gq1Bru7u1fb29vfdnZ2Pon4O7HrjpeeuzaczFDDcR/ScEa1zLmhlnn9kJY5rukHfOm8U9N/9Ocf6hEA/E8MtboAAAAASUVORK5CYII=);"

#define ARROWLEFT_ICON "background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAANCAYAAACzbK7QAAAAAXNSR0IArs4c6QAAAAZiS0dEAL8AvwC//ggzigAAAZdJREFUOMud1LFqk2EUBuDny58acLDWptJBimhjA0KkZCgKDoLo5gVkk5Ir6IVkL70GEWdBsriFEKh1VTupre3gYLB/jssfrSG1jWc5w8d533Pe9zsnmRIRUUIZJYyQp5Ry/xFpCniGq7iJK/iOT/iWUvo5K0F5SueXcQfPivwZr/E2Ir7OOkn5FPgcrmEV9/FwMBjcrdfrR5VKJcMX/IiI4QVwRzhJKeXplCxLeIAnuIfVlFJ1MBgMG41GHy+xh/MIchzjA47HBBU08BxP2+32Sq/Xy/r9foqIHEf4WOTzJBpiFy+wO5aohHmspJRuZVmmVquBZrOZRcQiFiMi/oV8eHiYIiL29/fhDUrlKdrZ3NyM7e3tVK1WbW1tWVtbi4g4Gb+fFQcHBynP8+E0k0fF+HsRsYDV5eXlpU6nk3W73Wi1WkdjTS8o0bsCb3Ta5AXcLgx+jI319fUbzWbzZGdnp4dXM5j827M0sWCXcB0beISVouD9DARjkj/fdGLZ5rBYgM8j/upoxkVLZ9yirPAnm+xo1lPxC9xTrtz4WAF1AAAAAElFTkSuQmCC);"

#define ARROWRIGHT_ICON "background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAANCAYAAACzbK7QAAAAAXNSR0IArs4c6QAAAAZiS0dEAL8AvwC//ggzigAAAapJREFUOMutlM9KW1EQxn9zvCnFyG0hSrmthhJQGxChlbpxJVknfYD4Gt0L2eUZ7lskkEUgUMFFG+gi/lloV7VBxD+LlKiX9ma6mZtKNgnRA4ezmTPf9803M8KUR1Ud4AABFIhFREfjZMrkM8AckAGeA3fAJXArIoNHARhzH1gG1oAXwDnwHTgDoodKXPJJVVOq+mzcNcavgPV+v/++1WptAe+AFSAL+KYQAM8YzQIL9roxIlLAGyDbbrezhULhk6peA5sGfACcqmpPRAaiqilgEfgILAHeGAAHpIF5IC0iO7lc7iafz1/W6/UG0AG+AGci8sczH9LAUiaT2RUR8X3/fpwXIjL0L4oir9ForIjIqqp+tkoIo2yr1eqec84LgiCaIL/rdDovK5XKRrfb9YvF4mmtVlu2lv0f+KBEm/amJijRLLBQKpW2m83m23K5fByG4T5wAZwAX4FfSYli4Ar4BhxNYLJnRD7EcRwHQdALw3AP+AEcAj8tXzycA+ukmQnnIumiDeC1EboGjg2kB/xNBm7aQRt2keW4NZDfIhI/xaoYVZzsosFo7D8N5K8UUKG/kgAAAABJRU5ErkJggg==);"

#define DIMMER_BG "background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAAZiS0dEAP8A/wD/oL2nkwAAAAlwSFlzAAALEwAACxMBAJqcGAAAAAd0SU1FB9gLCgMmI0krqZgAAAAZdEVYdENvbW1lbnQAQ3JlYXRlZCB3aXRoIEdJTVBXgQ4XAAAAHUlEQVQ4y2NkYGCoZ6AAMDFQCEYNGDVg1IDBYgAARyIAn4UMLlsAAAAASUVORK5CYII=);"

#define FAVICON "data:image/x-icon;base64,AAABAAEAEBAAAAEACABoBQAAFgAAACgAAAAQAAAAIAAAAAEACAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQEBAAGxsbAB0dHQA5Kx4ANywhAD0wJABAMSIAQDEjAEEyJABDMyUAQDQmAEM0JQAyMjIAQzQmAEY2JgA0NDQASzclAEY3KQBHNyoASDgoAEk4KQBFOCwATDkoAE86JQBKOSsATTonAFA8KQBMPCwASDwwAFE+KwBUPyoAPT09AFJBMABTQTAAVkIuAFZCLwBWQy8AV0Y2AF1ILwBfTDIAXEs6AF9NNABeTDoAY040AGhPMABjTTkAbVE5AGhSPQBrVTsAalU/AGxbSQBzXUAAeWJEAF9fXwB1ZEsAc2RPAHpoTwB8aU8Agm9XAIFuXQB/b14AgXJeAIp1WQCHdl8AinhjAIt4ZgCPeWYAi3lqAI97YgCPfGwAkX1sAJB/aQCYgmYAmYFqAJOCcQCCgoIAmohyAJqHdgCXiXYAmIp3AJeKeACZi3YAn4xxAJmLeACZi3kAmox+AI2NjQCekIIAo5N/AKOUiACnl38AqpeGAK2bhQComo0AqpuJALCciwCsnpIAs6GLALKijgC0opIAsKKUALGklwCyppIAuKWWALCmnQC1p5kAtqiWALWnnACxp54AsKifALSrogC5rJ8AuK2fAL+uoQC+sqYAwrWjAMK1qAC1tbUAx7epAMe4rgDIvLEAy7ywAMi+rgDIvbIAzL2xAMzCsgDMwbYAzsOwAM3DuQDSxLcA08W5AMTExADSyLcA1Mm9ANbNvQDUzcUA2M7DAM3NzQDf1swA39fNANfX1wDi28oA5d3OAOfe1QDd3d0A5t7XAN/f3wDn4NkA4ODgAObk3gDr5t0A5ubmAO7n4ADv6toA8uvcAPLs3ADv6+QA9e7eAPPu3wDw7OcA9O3mAPfv4QD27uYA8O/qAPjw5QD48esA9/byAPT29AD39/cA+Pj4APz59AD5+vcA/vz3APz//gD///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAr6mSkqivr6iXqa+pkpKpr6+MAQ+DqamDNYOokAIflK+viQBLH5KSH1YfklYMdamvr4kASx+Skh9WH5JWDHWpr6+MAQ+DqamDNYOokAIflK+vqZKSqK+vqJepr6mSkqmvrx0aFxgUEw4LDQkIBwYDrxsmVFFOTz89UDcQGRYSEQQgLZ6bmaFmjZ1/JyQjIh4FJTCgfTY4U5qFpECEho8pCiozpXosMXORRaJpcI5HKxU8NKZ+LkKfiEOKljlYiy8cbXesk2OArXJMeKpeRJw6IW55rphqo6diWmSre5WHPihogoF8dHZxZ2FcX1tSSUgyr2xva2VgXVlXVU1KRkE7r4YhAACAAQAAgAEAAIABAACAAQAAhiEAAIABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIABAAA="

static const char xhtml_header[] =
"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n"
"  \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
"<html xmlns=\"http://www.w3.org/1999/xhtml\" lang=\"en\" xml:lang=\"en\">\n"
"  <head>\n"
"  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />"
"  <title>%s [LV2 Plugin Doc]</title>\n"
"  <link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"" FAVICON "\" />\n"
"  <style type=\"text/css\">\n"
"body {font-size:12px; background:#aaa;}\n"
"h1, h2, h3 {margin:.25em .5em;}\n"
".plugintable {border:0px; border-spacing:0; border-collapse:separate; margin:0em auto;}\n"
".plugintable td div {min-height:20px;}\n"
".plugintable td {padding:.5em 2em 0em 2em; border-width:0; cursor:crosshair;}\n"
"td.bgL {background-position:left; padding-left:28px; background-repeat:no-repeat;}\n"
"td.bgR {background-position:right; padding-right:28px; background-repeat:no-repeat;}\n"
"td.midiport {border-bottom:2px solid blue;}\n"
"td.audioport {border-bottom:2px solid red;}\n"
"td.controlport {border-bottom:2px solid green;}\n"
"td.atomport {border-bottom:2px solid yellow;}\n"
"td.pbody {background:#cccccc; font-size:90%%; padding: .25em 1em;}\n"
".left {text-align:left;}\n"
".right {text-align:right;}\n"
".toplink {float:right; margin:1em 0 0 0;}\n"
".toplink a {color:black; text-decoration:none;}\n"

"p {margin: .25em 0;}\n"
"p.port {background-position:top left; line-height:24px; padding-left:28px; background-repeat:no-repeat; min-height:24px;}\n"
".midiport {"MIDI_ICON"}\n"
".audioport {"AUDIO_ICON"}\n"
".controlport {"CONTROL_ICON"}\n"
".atomport {"ATOM_ICON"}\n"

"dt,.desc {font-style:italic;}\n"
"dd {margin-left:1em; white-space: pre-line; white-space: pre; white-space: pre-wrap; word-wrap: break-word;}\n"
"dd ul {margin-left:1em; white-space: normal;}\n"
"ul {padding:0 0 0 1.25em; margin:0;}\n"

"p.backlink {margin: .5em 2.0em 0 1.0em; float:left;}\n"
"p.docs {max-width:30em;}\n"

"div.screenshot {text-align:center;}\n"
"img.screenshot {max-width:100%%; max-height:300px; margin:.5em auto .25em auto; cursor:pointer;}\n"

"#pluginmeta {margin:0 1em; padding:1em; border:1px solid #ccc; float:left; max-width:22em; background-color:#fff;}\n"
"#portwrapper {max-width:60em; margin:0em auto; float:left; background-color:#fff; border:1px solid #ccc;}\n"
"#plugintable {margin:1em 0; padding:1em;}\n"
"#portlist {margin:1em auto; padding:1em; min-width:45em;}\n"
"#lightbox {position:fixed; z-index:100; margin:0; top:0; left:0; width:100%%; height:100%%; text-align:center; "DIMMER_BG"}\n"
"#lightbox div {background:#fff; border-radius:10px; display:inline-block; margin:3em 0;}\n"
"#lightbox img {margin:1.5em; border:1px solid black;}\n"

"#inputportlist  {width:45%%; float:left;}\n"
"#outputportlist {width:45%%; float:right;}\n"

"td.ptbl div {position:relative;}\n"
"td.ptblLL div.portinfo {position:absolute; left:0px; background-position: top left; "ARROWLEFT_ICON"}\n"
"td.ptblRR div.portinfo {position:absolute; right:0px; background-position: top right; "ARROWRIGHT_ICON"}\n"
"div.portinfo {z-index:20; top:1.25em; background-color:#eee; background-repeat:no-repeat; border-radius:10px; padding: 1em; border:1px solid #888; min-width:18em; text-align:left;}\n"
"div.port {margin:.5em .5em 1em .5em; border-top:1px dotted #ccc; padding: 0 .5em; min-width:18em;}\n"

"span.portproperty {padding:0 .5em 0 0; border-bottom:1px dotted #ccc;}\n"
"table.portrange {width:100%%; margin-bottom:1em;  border:1px solid #888;}\n"

"td.ptbl{background:#cccccc; border-color: black; border-style: solid; background-clip: padding-box; position:relative;}\n"
"td.ptblTL{border-width: 1px 0px 0px 1px; border-radius:10px 0px 0px 0px;}\n"
"td.ptblTC{border-width: 1px 0px 0px 0px;}\n"
"td.ptblTR{border-width: 1px 1px 0px 0px; border-radius:0px 10px 0px 0px;}\n"
"td.ptblBL{border-width: 0px 0px 1px 1px; border-radius:0px 0px 0px 10px;}\n"
"td.ptblBC{border-width: 0px 0px 1px 0px;}\n"
"td.ptblBR{border-width: 0px 1px 1px 0px; border-radius:0px 0px 10px 0px;}\n"
"td.ptblRR{border-width: 0px 1px 0px 0px;}\n"
"td.ptblLL{border-width: 0px 0px 0px 1px;}\n"

"div.footer {margin:1em auto; font-size:80%%; color:#444; text-align:center; width:50%%;}\n"
"  </style>\n"

"  <script type=\"text/javascript\">\n"
"  //<![CDATA[\n"
"  function showdoc(p) {\n"
"    for(i=0; i< num_ports; ++i) {\n"
"      var el = document.getElementById('port'+i);\n"
"      if (!el) {continue};\n"
"      if (i==p) {el.style.display='block';}\n"
"      else {el.style.display='none';}\n"
"    }\n"
"  }\n"
"  function jumpto(p){window.location.hash='portdoc'+p;}\n"
"  function hideimg(){\n"
"    var lb = document.getElementById('lightbox');\n"
"    if (lb) {lb.style.display='none';};\n"
"  }\n"
"  function showimg(){\n"
"    var lb = document.getElementById('lightbox');\n"
"    if (lb) {lb.style.display='block';};\n"
"  }\n"
"  //]]>\n"
"  </script>\n"
"  </head>\n"
"  <body>\n";

static const char xhtml_indexhead[] =
"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n"
"  \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
"<html xmlns=\"http://www.w3.org/1999/xhtml\" lang=\"en\" xml:lang=\"en\">\n"
"  <head>\n"
"  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />"
"  <title>LV2 Plugin Documentation</title>\n"
"  <link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"" FAVICON "\" />\n"
"  <style type=\"text/css\">\n"
"body {font-size:12px; background:#aaa;}\n"
"h1, h2, h3 {margin:.25em 0;}\n"
"table {border:1px solid black; background:#fff; margin:1em auto; padding:.5em; border-spacing:0; border-collapse:separate;}\n"
"td {border-right:1px dotted black; border-top:1px solid black; margin:0; padding:.2em .5em;}\n"
"td.first, th.first {border-left:1px dotted black;}\n"
"th {border-right:1px dotted black; padding:.2em .5em;}\n"
"td.last {border-left:0px; border-right:0px;}\n"
".center {text-align:center;}\n"
"div.footer {margin:1em auto; font-size:80%%; color:#444; text-align:center; width:50%%;}\n"
"a.nl {color:black; text-decoration:none;}\n"
"  </style>\n"
"  </head>\n"
"  <body>\n"
"<!-- start of page !-->\n";

static const char xhtml_footer[] =
"<!-- end of page !-->\n"
"<div style=\"clear:both;\"></div>\n"
"<div class=\"footer\"><hr/>Created by <a href=\"https://github.com/x42/lv2toweb\">lv2toweb</a>. %s</div>\n"
" </body>\n"
" </html>\n";
