#include "jsmn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//testing the way to push using atom


typedef struct event_details   // struct that contains a 100 count character array that will be used to store data for each event
{
  char event_details[100];
}event_details;

static int jsoneq(const char *json, jsmntok_t *tok, const char *s)   // JSMN parser function to check if json formats are equa l
{
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

void get_end_time(char* event, char end_time[])  // Takes a formatted event string as input and gives out the end time of the event
{
  int j = 0;
  for(int i = 19; i < 24; i++)
  {
    end_time[j] = event[i];
    j += 1;
  }
  end_time[j] = '\0';

  return ;
}

void get_start_time(char* event, char start_time[])   // Takes a formatted event string as input and gives out the start time of the event
{

  int i = 35;
  while( event[i] != ',')
  {
    i++;
  }
  i += 20;

  int count = 0;
  int j = 0;
  while(count < 5)
  {
    start_time[j] = event[i];
    j += 1;
    i += 1;
    count += 1;

  }
  start_time[j] = '\0';
  return ;
}

void get_end_date(char* event, char end_date[])   // Takes in the formatted event string as input and gives out the end date of the event.
{
  //int j = 0;
/*  for(int i = 2; i < 12; i++)
  {
    end_date[j] = event[i];
    j += 1;
  }*/
  strncpy(end_date, event+8,10);
//  end_date[j] = 'W';
  //j++;
  end_date[10] = '\0';
  //printf("end date is %s", end_date);

  return;
}

void get_start_date(char* event, char start_date[])   // Takes in the formatted event string as an input and gives out the start date of the event.
{
  int i = 35;
  while( event[i] != ',')
  {
    i++;
  }
  i += 9;

  strncpy(start_date, event+i,10);
  start_date[10] = '\0';
  //printf("start date is %s", start_date);
  /*
  int count = 0;
  int j = 0;
  while(count != 10)
  {
    start_date[j] = event[i];
    printf("%c\n", start_date[j]);
    i += 1;
    j += 1;
    count += 1;
  }
  */

  //printf("start_string is %s", start_string);

  return;
}

char *replaceWord(const char *s, const char *oldW,       // Used to replace a substring in a given string by the input s.
                                 const char *newW)
{
    char *result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);

    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++)
    {
        if (strstr(&s[i], oldW) == &s[i])
        {
            cnt++;

            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }

    // Making new string of enough length
    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);

    i = 0;
    while (*s)
    {
        // compare the substring with the result
        if (strstr(s, oldW) == s)
        {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }

    result[i] = '\0';
    return result;
}

int main(int argc, char* argv[])
{

  static const char *js = "{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  "
    "\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";


  // we have to modify the json string upon receving it. There can't be nested keys, so those have to be modified to become arrays
  // format of events: events = {"summary": "Event name", "start": [dateTime(string),timeZone(string)], "end": [dateTime(string), TimeZone(string)]}
  static const char *all_events = "{\"events\":[{\"end\":\"2020-03-30T09:10:00-04:00\",\"event\":\"Stretching\",\"time\":\"2020-03-30T08:30:00-04:00\"},{\"end\":\"2020-03-30T10:20:00-04:00\",\"event\":\"Breakfast \",\"time\":\"2020-03-30T09:20:00-04:00\"},{\"end\":\"2020-03-30T12:00:00-04:00\",\"event\":\"Productive desk work\",\"time\":\"2020-03-30T10:30:00-04:00\"},{\"end\":\"2020-03-31T09:10:00-04:00\",\"event\":\"Stretching\",\"time\":\"2020-03-31T08:30:00-04:00\"},{\"end\":\"2020-03-31T10:20:00-04:00\",\"event\":\"Breakfast \",\"time\":\"2020-03-31T09:20:00-04:00\"},{\"end\":\"2020-03-31T12:00:00-04:00\",\"event\":\"Productive desk work\",\"time\":\"2020-03-31T10:30:00-04:00\"},{\"end\":\"2020-03-31T13:20:00-04:00\",\"event\":\"ECE 477-lec \",\"time\":\"2020-03-31T12:30:00-04:00\"},{\"end\":\"2020-04-01T12:00:00-04:00\",\"event\":\"Productive desk work\",\"time\":\"2020-04-01T10:30:00-04:00\"},{\"end\":\"2020-04-02T09:10:00-04:00\",\"event\":\"Stretching\",\"time\":\"2020-04-02T08:30:00-04:00\"},{\"end\":\"2020-04-02T10:20:00-04:00\",\"event\":\"Breakfast \",\"time\":\"2020-04-02T09:20:00-04:00\"},{\"end\":\"2020-04-02T12:00:00-04:00\",\"event\":\"Productive desk work\",\"time\":\"2020-04-02T10:30:00-04:00\"},{\"end\":\"2020-04-02T13:20:00-04:00\",\"event\":\"ECE 477-lec \",\"time\":\"2020-04-02T12:30:00-04:00\"},{\"end\":\"2020-04-03T09:10:00-04:00\",\"event\":\"Stretching\",\"time\":\"2020-04-03T08:30:00-04:00\"},{\"end\":\"2020-04-03T10:20:00-04:00\",\"event\":\"Breakfast \",\"time\":\"2020-04-03T09:20:00-04:00\"},{\"end\":\"2020-04-03T12:00:00-04:00\",\"event\":\"Productive desk work\",\"time\":\"2020-04-03T10:30:00-04:00\"},{\"end\":\"2020-04-04T09:10:00-04:00\",\"event\":\"Stretching\",\"time\":\"2020-04-04T08:30:00-04:00\"},{\"end\":\"2020-04-04T10:20:00-04:00\",\"event\":\"Breakfast \",\"time\":\"2020-04-04T09:20:00-04:00\"},{\"end\":\"2020-04-05T09:10:00-04:00\",\"event\":\"Stretching\",\"time\":\"2020-04-05T08:30:00-04:00\"},{\"end\":\"2020-04-05T10:20:00-04:00\",\"event\":\"Breakfast \",\"time\":\"2020-04-05T09:20:00-04:00\"}]}";
  jsmn_parser parser;
  jsmntok_t tokens[400];
  int num_parsed;
  jsmn_init(&parser);
  // js - pointer to JSON string
  // tokens - an array of tokens available
  // 10 - number of tokens available
  num_parsed = jsmn_parse(&parser, all_events, strlen(all_events), tokens, sizeof(tokens)/sizeof(tokens[0]));

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

  // Example parser code can be found on JSMN Github
  // Attempting to go through all iterations of events:
  int event_count = 0;            // used as a count in the while loop below to retrieve the correct number of events
  char temp_event[400];           // variables used to store unformatted event retrieved from parsing json string.
  char start_date[10];            // character array to store the start date of an event
  char end_date[10];              // character array to store the end date of an event
  char rando[10];
  char start_time[6];             // character array to store the start time of an event
  char end_time[6];               // character array to store the end time of an event
  char* str_replaced = NULL;      // pointer to a character to help with formatting of the parsed json string
  event_details event_list[100];  // array containing event_details structs
  int event_iter = 0;             // iterator used to go through event_list and place event_details




  for (int i = 1; i < num_parsed; i++) {
    if (jsoneq(all_events, &tokens[i], "events") == 0) {
      int j;
      //printf("array size is: %d", tokens[i+1].size); // There are 19 events currently.
      printf("- events:\n");
      if (tokens[i + 1].type != JSMN_ARRAY) {
        continue; // We expect groups to be an array of strings
      }
      j = 0;
      while(event_count != tokens[i + 1].size){
        jsmntok_t *g = &tokens[i + j + 2];

        if( (all_events + g->start)[0] == '{'){
          sprintf(temp_event,"%.*s\n", g->end - g->start, all_events + g->start);

          // Bottom three lines are not necessary anymore but get_start_time and get_end_time have to be modified if they are removed!!
          /*
          str_replaced = replaceWord(temp_event, "{\"end\":","{");
          str_replaced = replaceWord(str_replaced, "\"event\":\"", "\"");
          str_replaced = replaceWord(str_replaced, "\"time\":\"", "\"");
          */
          printf("event: %s", temp_event);
          sprintf(event_list[event_iter].event_details, "%s", temp_event);
          event_iter += 1;
          // remember to free str_replaced!!!!!
          //printf("start time: %s, end_time: %s, start_date: %s, end_date: %s\n", start_time, end_time, start_date, end_date);
          event_count += 1;
        }
        j += 1;
      }
      i += tokens[i + 1].size + 1;
    }else {
      continue;
    }
  }



  printf("event_list now contains all events as shown below: \n \n");
  for(int i = 0; i < 19; i++)
  {
    printf("event detail: %s", event_list[i].event_details);
    get_start_time(event_list[i].event_details, start_time);
    get_end_time(event_list[i].event_details, end_time);
    printf("      start time: %s, end time: %s\n", start_time, end_time);
    get_end_date(event_list[i].event_details, end_date);
    get_start_date(event_list[i].event_details, start_date);
    //printf("  start date is %s\n", start_date);
    printf("      start date: %s, end date: %s\n", start_date, end_date);
  }



  // we need some of getting
}
