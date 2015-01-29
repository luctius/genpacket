/*
 * File automatically generated by
 * gengen 1.4.3rc by Lorenzo Bettini 
 * http://www.gnu.org/software/gengen
 */

#include "var_declaration.h"


static char *
int_to_string(int i)
{
  static char buffer[256];

  snprintf (buffer, 255, "%d", i);

  return buffer;
}

void
generate_var_declaration(FILE *stream, struct var_declaration_gen_struct *record, unsigned int indent)
{
  char *indent_str;
  unsigned int i;

  indent_str = (char *) malloc (indent + 1);
  for (i = 0; i < indent; ++i)
    indent_str[i] = ' ';
  indent_str[indent] = '\0';
  indent = 0;

  fprintf (stream, "%s", (record->type ? record->type : ""));
  fprintf (stream, "%s", " ");
  if (record->pointer)
    {
      fprintf (stream, "%s", "*");
    }
  fprintf (stream, "%s", (record->member_name ? record->member_name : ""));
  if (record->array)
    {
      fprintf (stream, "%s", "[");
      fprintf (stream, "%d", record->size);
      fprintf (stream, "%s", "]");
    }
  fprintf (stream, "%s", ";");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);

  free (indent_str);
}

void
generatep_var_declaration(FILE *stream, unsigned int indent, short array, const char *member_name, short pointer, int size, const char *type)
{
  struct var_declaration_gen_struct record;
  
  record.array = array;
  record.member_name = member_name;
  record.pointer = pointer;
  record.size = size;
  record.type = type;

  generate_var_declaration (stream, &record, indent);
}

char *
genstring_var_declaration(struct var_declaration_gen_struct *record, unsigned int indent)
{
  char *indent_str, *output;
  unsigned int i;
  int len;

  indent_str = (char *) malloc (indent + 1);
  for (i = 0; i < indent; ++i)
    indent_str[i] = ' ';
  indent_str[indent] = '\0';

  len = strcnt_var_declaration (record, indent);
  output = (char *) malloc (len + 1);
  output[0] = '\0';

  if (record->type) strcat (output, record->type);
  strcat (output, " ");
  if (record->pointer)
    {
      strcat (output, "*");
    }
  if (record->member_name) strcat (output, record->member_name);
  if (record->array)
    {
      strcat (output, "[");
      strcat (output, int_to_string (record->size));
      strcat (output, "]");
    }
  strcat (output, ";");
  strcat (output, "\n");
  strcat (output, indent_str);

  free (indent_str);

  return output;
}

char *
genstringp_var_declaration(unsigned int indent, short array, const char *member_name, short pointer, int size, const char *type)
{
  struct var_declaration_gen_struct record;
  
  record.array = array;
  record.member_name = member_name;
  record.pointer = pointer;
  record.size = size;
  record.type = type;

  return genstring_var_declaration (&record, indent);
}

int
strcnt_var_declaration(struct var_declaration_gen_struct *record, unsigned int indent)
{
  int length = 0;
  
  length += (record->type ? strlen (record->type) : 0) * 1;
if (record->pointer)
    {
      

    }
  length += (record->member_name ? strlen (record->member_name) : 0) * 1;
if (record->array)
    {
      length += strlen (int_to_string (record->size)) * 1;
    }

  return length + 8;
}

void
init_var_declaration_gen_struct(struct var_declaration_gen_struct *record)
{
  record->array = 0;
  record->member_name = 0;
  record->pointer = 0;
  record->size = 0;
  record->type = 0;
}

