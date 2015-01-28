/*
 * File automatically generated by
 * gengen 1.4.3rc by Lorenzo Bettini 
 * http://www.gnu.org/software/gengen
 */

#include "cqc.h"

void
generate_cqc(FILE *stream, struct cqc_gen_struct *record, unsigned int indent)
{
  char *indent_str;
  unsigned int i;

  indent_str = (char *) malloc (indent + 1);
  for (i = 0; i < indent; ++i)
    indent_str[i] = ' ';
  indent_str[indent] = '\0';
  indent = 0;

  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#ifndef ");
  fprintf (stream, "%s", (record->ifndefname ? record->ifndefname : ""));
  fprintf (stream, "%s", "_CQC_H");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define ");
  fprintf (stream, "%s", (record->ifndefname ? record->ifndefname : ""));
  fprintf (stream, "%s", "_CQC_H");
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
  fprintf (stream, "%s", "#define static_assert(pred, msg) _Static_assert( (pred), msg);");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "struct cqc {");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    int head;");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    int tail;");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    int size;");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "};");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  if (record->modulo)
    {
      fprintf (stream, "%s", "//#define CQC_USE_MODULO /* Define this if you need arbitrary size queues instead of power of two sizes. */");
    }
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#ifdef CQC_USE_MODULO");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    /* Init Circular buffer. In the case of Module, given size is the size of the buffer. */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_init(cqc, sz) do { (cqc).size = sz; (cqc).tail = 0; (cqc).head = 0; } while (0)");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    /* Size given with init. */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_qsz(cqc)        ( (cqc).size )");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    /* Number of Items in the buffer. */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_cnt(cqc)        ( ((cqc).head-(cqc).tail) % ((cqc).size) )");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    /* Add new item, returns a free index and updates bookkeeping. */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_put(cqc)        ( ((cqc).head++)          % ((cqc).size) )");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    /* Retreive item, returns an used index and updates bookkeeping. */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_get(cqc)        ( ((cqc).tail++)          % ((cqc).size) )");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    /* Retreive item idx, does not touch bookkeeping. */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_peek(cqc, idx)  ( ((cqc).tail+idx)        % ((cqc).size) )");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    /* Space left in the buffer. */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_space(cqc)      ( ((cqc).size)            - (cqc_cnt(cqc)) )");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_cnt_to_end(cqc)     ({int end = (cqc.size) - (cqc.tail); int n = ((cqc.head) + end) & ((cqc.size)-1); n < end ? n : end;})");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_space_to_end(cqc)   ({int end = (cqc.size) - 1 - (cqc.head); int n = (end + (cqc.tail)) & ((cqc.size)-1); n <= end ? n : end+1;})");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#elif !defined(CQC_USE_MODULO)");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    /* Init Circular buffer. In the case of AND, given size is the nearest power of 2 of the given size. cqc_qsz should be used to initialise the buffer. */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_init(cqc, sz) do { static_assert(sz != 0, \"cqc sz is zero\"); static_assert( (sz & (sz-1) ) == 0, \"cqc sz is not a power of 2.\"); (cqc).size = sz-1; (cqc).tail = 0; (cqc).head = 0; } while (0)");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    /* Size calculated in init. */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_qsz(cqc)        ( (cqc).size+1 )");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    /* Number of Items in the buffer. */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_cnt(cqc)        ( ((cqc).head-(cqc).tail) & ((cqc).size) )");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    /* Retreive item, returns an used index and updates bookkeeping. */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_put(cqc)        ( ((cqc).head++)          & ((cqc).size) )");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    /* Retreive item, returns an used index and updates bookkeeping. */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_get(cqc)        ( ((cqc).tail++)          & ((cqc).size) )");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    /* Retreive item idx, does not touch bookkeeping. */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_peek(cqc, idx)  ( ((cqc).tail+idx)        & ((cqc).size) )");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "    /* Space left in the buffer. */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_space(cqc)      ( ((cqc).size)            - (cqc_cnt(cqc)) )");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_cnt_to_end(cqc)     ({int end = (cqc.size) - (cqc.tail); int n = ((cqc.head) + end) & ((cqc.size)-1); n < end ? n : end;})");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#define cqc_space_to_end(cqc)   ({int end = (cqc.size) - 1 - (cqc.head); int n = (end + (cqc.tail)) & ((cqc.size)-1); n <= end ? n : end+1;})");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "#endif /* CQC_USE_MODULO */");
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
  fprintf (stream, "%s", "_CQC_H */");
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);
  fprintf (stream, "%s", "\n");
  fprintf (stream, "%s", indent_str);

  free (indent_str);
}

void
generatep_cqc(FILE *stream, unsigned int indent, const char *ifndefname, short modulo)
{
  struct cqc_gen_struct record;
  
  record.ifndefname = ifndefname;
  record.modulo = modulo;

  generate_cqc (stream, &record, indent);
}

char *
genstring_cqc(struct cqc_gen_struct *record, unsigned int indent)
{
  char *indent_str, *output;
  unsigned int i;
  int len;

  indent_str = (char *) malloc (indent + 1);
  for (i = 0; i < indent; ++i)
    indent_str[i] = ' ';
  indent_str[indent] = '\0';

  len = strcnt_cqc (record, indent);
  output = (char *) malloc (len + 1);
  output[0] = '\0';

  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#ifndef ");
  if (record->ifndefname) strcat (output, record->ifndefname);
  strcat (output, "_CQC_H");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define ");
  if (record->ifndefname) strcat (output, record->ifndefname);
  strcat (output, "_CQC_H");
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
  strcat (output, "#define static_assert(pred, msg) _Static_assert( (pred), msg);");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "struct cqc {");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    int head;");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    int tail;");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    int size;");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "};");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  if (record->modulo)
    {
      strcat (output, "//#define CQC_USE_MODULO /* Define this if you need arbitrary size queues instead of power of two sizes. */");
    }
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#ifdef CQC_USE_MODULO");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    /* Init Circular buffer. In the case of Module, given size is the size of the buffer. */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_init(cqc, sz) do { (cqc).size = sz; (cqc).tail = 0; (cqc).head = 0; } while (0)");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    /* Size given with init. */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_qsz(cqc)        ( (cqc).size )");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    /* Number of Items in the buffer. */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_cnt(cqc)        ( ((cqc).head-(cqc).tail) % ((cqc).size) )");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    /* Add new item, returns a free index and updates bookkeeping. */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_put(cqc)        ( ((cqc).head++)          % ((cqc).size) )");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    /* Retreive item, returns an used index and updates bookkeeping. */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_get(cqc)        ( ((cqc).tail++)          % ((cqc).size) )");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    /* Retreive item idx, does not touch bookkeeping. */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_peek(cqc, idx)  ( ((cqc).tail+idx)        % ((cqc).size) )");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    /* Space left in the buffer. */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_space(cqc)      ( ((cqc).size)            - (cqc_cnt(cqc)) )");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_cnt_to_end(cqc)     ({int end = (cqc.size) - (cqc.tail); int n = ((cqc.head) + end) & ((cqc.size)-1); n < end ? n : end;})");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_space_to_end(cqc)   ({int end = (cqc.size) - 1 - (cqc.head); int n = (end + (cqc.tail)) & ((cqc.size)-1); n <= end ? n : end+1;})");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#elif !defined(CQC_USE_MODULO)");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    /* Init Circular buffer. In the case of AND, given size is the nearest power of 2 of the given size. cqc_qsz should be used to initialise the buffer. */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_init(cqc, sz) do { static_assert(sz != 0, \"cqc sz is zero\"); static_assert( (sz & (sz-1) ) == 0, \"cqc sz is not a power of 2.\"); (cqc).size = sz-1; (cqc).tail = 0; (cqc).head = 0; } while (0)");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    /* Size calculated in init. */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_qsz(cqc)        ( (cqc).size+1 )");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    /* Number of Items in the buffer. */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_cnt(cqc)        ( ((cqc).head-(cqc).tail) & ((cqc).size) )");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    /* Retreive item, returns an used index and updates bookkeeping. */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_put(cqc)        ( ((cqc).head++)          & ((cqc).size) )");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    /* Retreive item, returns an used index and updates bookkeeping. */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_get(cqc)        ( ((cqc).tail++)          & ((cqc).size) )");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    /* Retreive item idx, does not touch bookkeeping. */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_peek(cqc, idx)  ( ((cqc).tail+idx)        & ((cqc).size) )");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "    /* Space left in the buffer. */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_space(cqc)      ( ((cqc).size)            - (cqc_cnt(cqc)) )");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_cnt_to_end(cqc)     ({int end = (cqc.size) - (cqc.tail); int n = ((cqc.head) + end) & ((cqc.size)-1); n < end ? n : end;})");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#define cqc_space_to_end(cqc)   ({int end = (cqc.size) - 1 - (cqc.head); int n = (end + (cqc.tail)) & ((cqc.size)-1); n <= end ? n : end+1;})");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "#endif /* CQC_USE_MODULO */");
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
  strcat (output, "_CQC_H */");
  strcat (output, "\n");
  strcat (output, indent_str);
  strcat (output, "\n");
  strcat (output, indent_str);

  free (indent_str);

  return output;
}

char *
genstringp_cqc(unsigned int indent, const char *ifndefname, short modulo)
{
  struct cqc_gen_struct record;
  
  record.ifndefname = ifndefname;
  record.modulo = modulo;

  return genstring_cqc (&record, indent);
}

int
strcnt_cqc(struct cqc_gen_struct *record, unsigned int indent)
{
  int length = 0;
  
  length += (record->ifndefname ? strlen (record->ifndefname) : 0) * 3;
if (record->modulo)
    {
      

    }

  return length + 3127;
}

void
init_cqc_gen_struct(struct cqc_gen_struct *record)
{
  record->ifndefname = 0;
  record->modulo = 0;
}

