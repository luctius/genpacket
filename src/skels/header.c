/*
 * File automatically generated by
 * gengen 1.4.3rc by Lorenzo Bettini 
 * http://www.gnu.org/software/gengen
 */

#include "header.h"


static void
generate_string(const char *s, FILE *stream, unsigned int indent)
{
  const char *sub = s;
  char *indent_str;
  unsigned int i;

  if (!indent || strchr (s, '\n') == 0)
    {
      fprintf (stream, "%s", s);
      return;
    }

  indent_str = (char *) malloc (indent + 1);
  for (i = 0; i < indent; ++i)
    indent_str[i] = ' ';
  indent_str[indent] = '\0';

  while ( (sub=strchr (s, '\n')) != 0 )
    {
      fwrite (s, sizeof(char), (sub-s)+1, stream);
      s = sub+1;
      if (s[0])
        fprintf (stream, "%s", indent_str);
    }
  if (s[0])
    fwrite (s, sizeof(char), strlen (s), stream);

  free (indent_str);
}


static char *
int_to_string(int i)
{
  static char buffer[256];

  snprintf (buffer, 255, "%d", i);

  return buffer;
}

void
generate_header(FILE *stream, struct header_gen_struct *record, unsigned int indent)
{
  char *indent_str;
  unsigned int i;

  indent_str = (char *) malloc (indent + 1);
  for (i = 0; i < indent; ++i)
    indent_str[i] = ' ';
  indent_str[indent] = '\0';
  indent = 0;

  fprintf (stream, "%s", "#ifndef ");
  fprintf (stream, "%s", (record->ifndefname ? record->ifndefname : ""));
  fprintf (stream, "%s", "_H");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define ");
  fprintf (stream, "%s", (record->ifndefname ? record->ifndefname : ""));
  fprintf (stream, "%s", "_H");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "/* If we use autoconf.  */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#ifdef HAVE_CONFIG_H");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#include \"config.h\"");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#endif");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#include <stdint.h>");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#ifdef __cplusplus");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "extern \"C\" {");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#endif /* __cplusplus */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define ");
  fprintf (stream, "%s", (record->ifndefname ? record->ifndefname : ""));
  fprintf (stream, "%s", "_NRO_PIPES (");
  fprintf (stream, "%d", record->nro_pipes);
  fprintf (stream, "%s", ")");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "enum ");
  fprintf (stream, "%s", (record->prefix ? record->prefix : ""));
  fprintf (stream, "%s", "_direction {");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  indent = 4;
  fprintf (stream, "%s", "    ");
  fprintf (stream, "%s", (record->ifndefname ? record->ifndefname : ""));
  fprintf (stream, "%s", "_R=1,");
  indent = 0;
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  indent = 4;
  fprintf (stream, "%s", "    ");
  fprintf (stream, "%s", (record->ifndefname ? record->ifndefname : ""));
  fprintf (stream, "%s", "_W=2,");
  indent = 0;
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "};");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  if (record->packets)
    generate_string (record->packets, stream, indent + strlen (indent_str));
  else
    generate_packets (stream, record, indent + strlen (indent_str));
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  if (record->receive_functions)
    generate_string (record->receive_functions, stream, indent + strlen (indent_str));
  else
    generate_receive_functions (stream, record, indent + strlen (indent_str));
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  if (record->send_functions)
    generate_string (record->send_functions, stream, indent + strlen (indent_str));
  else
    generate_send_functions (stream, record, indent + strlen (indent_str));
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "struct ");
  fprintf (stream, "%s", (record->prefix ? record->prefix : ""));
  fprintf (stream, "%s", "_params {");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    int pipe_fds[");
  fprintf (stream, "%s", (record->ifndefname ? record->ifndefname : ""));
  fprintf (stream, "%s", "_NRO_PIPES];");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    enum ");
  fprintf (stream, "%s", (record->prefix ? record->prefix : ""));
  fprintf (stream, "%s", "_direction pipe_dir[");
  fprintf (stream, "%s", (record->ifndefname ? record->ifndefname : ""));
  fprintf (stream, "%s", "_NRO_PIPES];");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    void *private_ctx;");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "};");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "int ");
  fprintf (stream, "%s", (record->prefix ? record->prefix : ""));
  fprintf (stream, "%s", "_init(struct ");
  fprintf (stream, "%s", (record->prefix ? record->prefix : ""));
  fprintf (stream, "%s", "_params *params);");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "int ");
  fprintf (stream, "%s", (record->prefix ? record->prefix : ""));
  fprintf (stream, "%s", "_process();");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#ifdef __cplusplus");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "}");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#endif /* __cplusplus */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#endif /* ");
  fprintf (stream, "%s", (record->ifndefname ? record->ifndefname : ""));
  fprintf (stream, "%s", "_H */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);

  free (indent_str);
}

void
generatep_header(FILE *stream, unsigned int indent, const char *ifndefname, int nro_pipes, const char *packets, const char *prefix, const char *receive_functions, const char *send_functions)
{
  struct header_gen_struct record;
  
  record.ifndefname = ifndefname;
  record.nro_pipes = nro_pipes;
  record.packets = packets;
  record.prefix = prefix;
  record.receive_functions = receive_functions;
  record.send_functions = send_functions;

  generate_header (stream, &record, indent);
}

char *
genstring_header(struct header_gen_struct *record, unsigned int indent)
{
  char *indent_str, *output;
  unsigned int i;
  int len;

  indent_str = (char *) malloc (indent + 1);
  for (i = 0; i < indent; ++i)
    indent_str[i] = ' ';
  indent_str[indent] = '\0';

  len = strcnt_header (record, indent);
  output = (char *) malloc (len + 1);
  output[0] = '\0';

  strcat (output, "#ifndef ");
  if (record->ifndefname) strcat (output, record->ifndefname);
  strcat (output, "_H");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define ");
  if (record->ifndefname) strcat (output, record->ifndefname);
  strcat (output, "_H");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "/* If we use autoconf.  */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#ifdef HAVE_CONFIG_H");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#include \"config.h\"");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#endif");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#include <stdint.h>");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#ifdef __cplusplus");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "extern \"C\" {");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#endif /* __cplusplus */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define ");
  if (record->ifndefname) strcat (output, record->ifndefname);
  strcat (output, "_NRO_PIPES (");
  strcat (output, int_to_string (record->nro_pipes));
  strcat (output, ")");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "enum ");
  if (record->prefix) strcat (output, record->prefix);
  strcat (output, "_direction {");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    ");
  if (record->ifndefname) strcat (output, record->ifndefname);
  strcat (output, "_R=1,");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    ");
  if (record->ifndefname) strcat (output, record->ifndefname);
  strcat (output, "_W=2,");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "};");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  if (record->packets) strcat (output, record->packets);
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  if (record->receive_functions) strcat (output, record->receive_functions);
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  if (record->send_functions) strcat (output, record->send_functions);
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "struct ");
  if (record->prefix) strcat (output, record->prefix);
  strcat (output, "_params {");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    int pipe_fds[");
  if (record->ifndefname) strcat (output, record->ifndefname);
  strcat (output, "_NRO_PIPES];");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    enum ");
  if (record->prefix) strcat (output, record->prefix);
  strcat (output, "_direction pipe_dir[");
  if (record->ifndefname) strcat (output, record->ifndefname);
  strcat (output, "_NRO_PIPES];");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    void *private_ctx;");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "};");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "int ");
  if (record->prefix) strcat (output, record->prefix);
  strcat (output, "_init(struct ");
  if (record->prefix) strcat (output, record->prefix);
  strcat (output, "_params *params);");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "int ");
  if (record->prefix) strcat (output, record->prefix);
  strcat (output, "_process();");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#ifdef __cplusplus");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "}");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#endif /* __cplusplus */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#endif /* ");
  if (record->ifndefname) strcat (output, record->ifndefname);
  strcat (output, "_H */");
  strcat (output, "\n");
  strcat (output, indent_str);

  free (indent_str);

  return output;
}

char *
genstringp_header(unsigned int indent, const char *ifndefname, int nro_pipes, const char *packets, const char *prefix, const char *receive_functions, const char *send_functions)
{
  struct header_gen_struct record;
  
  record.ifndefname = ifndefname;
  record.nro_pipes = nro_pipes;
  record.packets = packets;
  record.prefix = prefix;
  record.receive_functions = receive_functions;
  record.send_functions = send_functions;

  return genstring_header (&record, indent);
}

int
strcnt_header(struct header_gen_struct *record, unsigned int indent)
{
  int length = 0;
  
  length += (record->ifndefname ? strlen (record->ifndefname) : 0) * 8;
  length += strlen (int_to_string (record->nro_pipes)) * 1;
  length += (record->prefix ? strlen (record->prefix) : 0) * 6;
  length += (record->packets ? strlen (record->packets) : 0) * 1;
  length += (record->receive_functions ? strlen (record->receive_functions) : 0) * 1;
  length += (record->send_functions ? strlen (record->send_functions) : 0) * 1;

  return length + 556;
}

void
init_header_gen_struct(struct header_gen_struct *record)
{
  record->ifndefname = 0;
  record->nro_pipes = 0;
  record->packets = 0;
  record->prefix = 0;
  record->receive_functions = 0;
  record->send_functions = 0;
}

