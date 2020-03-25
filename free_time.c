#include "jsmn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

int main(int argc, char* argv[])
{

  static const char *js = "{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  "
    "\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";


  jsmn_parser parser;
  jsmntok_t tokens[180];
  int num_parsed;
  jsmn_init(&parser);
  // js - pointer to JSON string
  // tokens - an array of tokens available
  // 10 - number of tokens available
  num_parsed = jsmn_parse(&parser, js, strlen(js), tokens, sizeof(tokens)/sizeof(tokens[0]));

  /* Error occured:
     Not enough tokens were provided:
     JSMN_ERROR_NOMEM = -1,
     Invalid character inside JSON string:
     JSMN_ERROR_INVAL = -2,
     The string is not a full JSON packet, more bytes expected:
     JSMN_ERROR_PART = -3
  */
  if (num_parsed < 0) {
    printf("Failed to parse JSON: %d\n", num_parsed);
    return 1;
  }

  /* Assume the top-level element is an object */
  if (num_parsed < 1 || tokens[0].type != JSMN_OBJECT) {
    printf("Object expected\n");
    return 1;
  }

  //printf("first token is: %s", tokens[0]);
  for (int i = 1; i < num_parsed; i++) {
    if (jsoneq(js, &tokens[i], "user") == 0) {
      /* We may use strndup() to fetch string value */
      printf("- User: %.*s\n", tokens[i + 1].end - tokens[i + 1].start,
             js + tokens[i + 1].start);
      i++;
    } else if (jsoneq(js, &tokens[i], "admin") == 0) {
      /* We may additionally check if the value is either "true" or "false" */
      printf("- Admin: %.*s\n", tokens[i + 1].end - tokens[i + 1].start,
             js + tokens[i + 1].start);
      i++;
    } else if (jsoneq(js, &tokens[i], "uid") == 0) {
      /* We may want to do strtol() here to get numeric value */
      printf("- UID: %.*s\n", tokens[i + 1].end - tokens[i + 1].start,
             js + tokens[i + 1].start);
      i++;
    } else if (jsoneq(js, &tokens[i], "groups") == 0) {
      int j;
      printf("- Groups:\n");
      if (tokens[i + 1].type != JSMN_ARRAY) {
        continue; /* We expect groups to be an array of strings */
      }
      for (j = 0; j < tokens[i + 1].size; j++) {
        jsmntok_t *g = &tokens[i + j + 2];
        printf("  * %.*s\n", g->end - g->start, js + g->start);
      }
      i += tokens[i + 1].size + 1;
    } else {
      printf("Unexpected key: %.*s\n", tokens[i].end - tokens[i].start,
             js + tokens[i].start);
    }
  }


}
