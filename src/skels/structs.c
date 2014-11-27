/*
 * File automatically generated by
 * gengen 1.4.3rc by Lorenzo Bettini 
 * http://www.gnu.org/software/gengen
 */

#include "structs.h"


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

void
generate_structs(FILE *stream, struct structs_gen_struct *record, unsigned int indent)
{
  char *indent_str;
  unsigned int i;

  indent_str = (char *) malloc (indent + 1);
  for (i = 0; i < indent; ++i)
    indent_str[i] = ' ';
  indent_str[indent] = '\0';
  indent = 0;

  fprintf (stream, "%s", "struct ");
  fprintf (stream, "%s", (record->struct_name ? record->struct_name : ""));
  fprintf (stream, "%s", " {");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  indent = 4;
  fprintf (stream, "%s", "    ");
  if (record->struct_member)
    generate_string (record->struct_member, stream, indent + strlen (indent_str));
  else
    generate_struct_member (stream, record, indent + strlen (indent_str));
  indent = 0;
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "};");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);

  free (indent_str);
}

void
generatep_structs(FILE *stream, unsigned int indent, const char *struct_member, const char *struct_name)
{
  struct structs_gen_struct record;
  
  record.struct_member = struct_member;
  record.struct_name = struct_name;

  generate_structs (stream, &record, indent);
}

char *
genstring_structs(struct structs_gen_struct *record, unsigned int indent)
{
  char *indent_str, *output;
  unsigned int i;
  int len;

  indent_str = (char *) malloc (indent + 1);
  for (i = 0; i < indent; ++i)
    indent_str[i] = ' ';
  indent_str[indent] = '\0';

  len = strcnt_structs (record, indent);
  output = (char *) malloc (len + 1);
  output[0] = '\0';

  strcat (output, "struct ");
  if (record->struct_name) strcat (output, record->struct_name);
  strcat (output, " {");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    ");
  if (record->struct_member) strcat (output, record->struct_member);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "};");
  strcat (output, "\n");
  strcat (output, indent_str);

  free (indent_str);

  return output;
}

char *
genstringp_structs(unsigned int indent, const char *struct_member, const char *struct_name)
{
  struct structs_gen_struct record;
  
  record.struct_member = struct_member;
  record.struct_name = struct_name;

  return genstring_structs (&record, indent);
}

int
strcnt_structs(struct structs_gen_struct *record, unsigned int indent)
{
  int length = 0;
  
  length += (record->struct_name ? strlen (record->struct_name) : 0) * 1;
  length += (record->struct_member ? strlen (record->struct_member) : 0) * 1;

  return length + 24;
}

void
init_structs_gen_struct(struct structs_gen_struct *record)
{
  record->struct_member = 0;
  record->struct_name = 0;
}

