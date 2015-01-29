/*
 * File automatically generated by
 * gengen 1.4.3rc by Lorenzo Bettini 
 * http://www.gnu.org/software/gengen
 */

#include "fixed_packet_test_function_impl.h"


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
generate_fixed_packet_test_function_impl(FILE *stream, struct fixed_packet_test_function_impl_gen_struct *record, unsigned int indent)
{
  char *indent_str;
  unsigned int i;

  indent_str = (char *) malloc (indent + 1);
  for (i = 0; i < indent; ++i)
    indent_str[i] = ' ';
  indent_str[indent] = '\0';
  indent = 0;

  fprintf (stream, "%s", "static bool test_");
  fprintf (stream, "%s", (record->name ? record->name : ""));
  fprintf (stream, "%s", "(struct ");
  fprintf (stream, "%s", (record->prefix ? record->prefix : ""));
  fprintf (stream, "%s", "_ctx *ctx, int head) {");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    int cnt = cqc_cnt(ctx->recv_buff_cqc);");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    if (cnt < ");
  fprintf (stream, "%d", record->packet_size);
  fprintf (stream, "%s", ") return -1;");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    int retval = ");
  fprintf (stream, "%d", record->packet_size);
  fprintf (stream, "%s", ";");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    struct ");
  fprintf (stream, "%s", (record->name ? record->name : ""));
  fprintf (stream, "%s", " *packet = (struct ");
  fprintf (stream, "%s", (record->name ? record->name : ""));
  fprintf (stream, "%s", " *) &ctx->recv_buff[head];");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  indent = 4;
  fprintf (stream, "%s", "    ");
  if (record->test_frame_fixed)
    generate_string (record->test_frame_fixed, stream, indent + strlen (indent_str));
  else
    generate_test_frame_fixed (stream, record, indent + strlen (indent_str));
  indent = 0;
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    return retval;");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "}");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);

  free (indent_str);
}

void
generatep_fixed_packet_test_function_impl(FILE *stream, unsigned int indent, const char *name, int packet_size, const char *prefix, const char *test_frame_fixed)
{
  struct fixed_packet_test_function_impl_gen_struct record;
  
  record.name = name;
  record.packet_size = packet_size;
  record.prefix = prefix;
  record.test_frame_fixed = test_frame_fixed;

  generate_fixed_packet_test_function_impl (stream, &record, indent);
}

char *
genstring_fixed_packet_test_function_impl(struct fixed_packet_test_function_impl_gen_struct *record, unsigned int indent)
{
  char *indent_str, *output;
  unsigned int i;
  int len;

  indent_str = (char *) malloc (indent + 1);
  for (i = 0; i < indent; ++i)
    indent_str[i] = ' ';
  indent_str[indent] = '\0';

  len = strcnt_fixed_packet_test_function_impl (record, indent);
  output = (char *) malloc (len + 1);
  output[0] = '\0';

  strcat (output, "static bool test_");
  if (record->name) strcat (output, record->name);
  strcat (output, "(struct ");
  if (record->prefix) strcat (output, record->prefix);
  strcat (output, "_ctx *ctx, int head) {");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    int cnt = cqc_cnt(ctx->recv_buff_cqc);");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    if (cnt < ");
  strcat (output, int_to_string (record->packet_size));
  strcat (output, ") return -1;");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    int retval = ");
  strcat (output, int_to_string (record->packet_size));
  strcat (output, ";");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    struct ");
  if (record->name) strcat (output, record->name);
  strcat (output, " *packet = (struct ");
  if (record->name) strcat (output, record->name);
  strcat (output, " *) &ctx->recv_buff[head];");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    ");
  if (record->test_frame_fixed) strcat (output, record->test_frame_fixed);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    return retval;");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "}");
  strcat (output, "\n");
  strcat (output, indent_str);

  free (indent_str);

  return output;
}

char *
genstringp_fixed_packet_test_function_impl(unsigned int indent, const char *name, int packet_size, const char *prefix, const char *test_frame_fixed)
{
  struct fixed_packet_test_function_impl_gen_struct record;
  
  record.name = name;
  record.packet_size = packet_size;
  record.prefix = prefix;
  record.test_frame_fixed = test_frame_fixed;

  return genstring_fixed_packet_test_function_impl (&record, indent);
}

int
strcnt_fixed_packet_test_function_impl(struct fixed_packet_test_function_impl_gen_struct *record, unsigned int indent)
{
  int length = 0;
  
  length += (record->name ? strlen (record->name) : 0) * 3;
  length += (record->prefix ? strlen (record->prefix) : 0) * 1;
  length += strlen (int_to_string (record->packet_size)) * 2;
  length += (record->test_frame_fixed ? strlen (record->test_frame_fixed) : 0) * 1;

  return length + 242;
}

void
init_fixed_packet_test_function_impl_gen_struct(struct fixed_packet_test_function_impl_gen_struct *record)
{
  record->name = 0;
  record->packet_size = 0;
  record->prefix = 0;
  record->test_frame_fixed = 0;
}

