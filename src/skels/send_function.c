/*
 * File automatically generated by
 * gengen 1.4.3rc by Lorenzo Bettini 
 * http://www.gnu.org/software/gengen
 */

#include "send_function.h"

void
generate_send_function(FILE *stream, struct send_function_gen_struct *record, unsigned int indent)
{
  char *indent_str;
  unsigned int i;

  indent_str = (char *) malloc (indent + 1);
  for (i = 0; i < indent; ++i)
    indent_str[i] = ' ';
  indent_str[indent] = '\0';
  indent = 0;

  fprintf (stream, "%s", "int ");
  fprintf (stream, "%s", (record->prefix ? record->prefix : ""));
  fprintf (stream, "%s", "_");
  fprintf (stream, "%s", (record->packet_name ? record->packet_name : ""));
  fprintf (stream, "%s", "_send(struct ");
  fprintf (stream, "%s", (record->packet_name ? record->packet_name : ""));
  fprintf (stream, "%s", " *packet);");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);

  free (indent_str);
}

void
generatep_send_function(FILE *stream, unsigned int indent, const char *packet_name, const char *prefix)
{
  struct send_function_gen_struct record;
  
  record.packet_name = packet_name;
  record.prefix = prefix;

  generate_send_function (stream, &record, indent);
}

char *
genstring_send_function(struct send_function_gen_struct *record, unsigned int indent)
{
  char *indent_str, *output;
  unsigned int i;
  int len;

  indent_str = (char *) malloc (indent + 1);
  for (i = 0; i < indent; ++i)
    indent_str[i] = ' ';
  indent_str[indent] = '\0';

  len = strcnt_send_function (record, indent);
  output = (char *) malloc (len + 1);
  output[0] = '\0';

  strcat (output, "int ");
  if (record->prefix) strcat (output, record->prefix);
  strcat (output, "_");
  if (record->packet_name) strcat (output, record->packet_name);
  strcat (output, "_send(struct ");
  if (record->packet_name) strcat (output, record->packet_name);
  strcat (output, " *packet);");
  strcat (output, "\n");
  strcat (output, indent_str);

  free (indent_str);

  return output;
}

char *
genstringp_send_function(unsigned int indent, const char *packet_name, const char *prefix)
{
  struct send_function_gen_struct record;
  
  record.packet_name = packet_name;
  record.prefix = prefix;

  return genstring_send_function (&record, indent);
}

int
strcnt_send_function(struct send_function_gen_struct *record, unsigned int indent)
{
  int length = 0;
  
  length += (record->prefix ? strlen (record->prefix) : 0) * 1;
  length += (record->packet_name ? strlen (record->packet_name) : 0) * 2;

  return length + 31;
}

void
init_send_function_gen_struct(struct send_function_gen_struct *record)
{
  record->packet_name = 0;
  record->prefix = 0;
}
