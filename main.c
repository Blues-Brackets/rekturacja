#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

struct message
{
  uint8_t id;
  uint16_t length;
  uint8_t data[8];
};

int parse_message(uint8_t *message, int length, struct message *msg)
{
  int i;
  msg->id = message[0];
  msg->length = message[1];
  msg->length += message[2] * 256;

  for (i = 0; i < msg->length; i++)
  {
    msg->data[i] = message[2 + i];
  }
  return 0;
}

int main()
{
  uint8_t message[] = {0x01, 0x02, 0x00, 0x02, 0x03};
  struct message msg;

  int ret = parse_message(message, sizeof(message), &msg);

  assert(("Failed to parse message", ret == 0));
  assert(("ID is incorrect", msg.id == 0x01));
  assert(("Length is incorrect", msg.length <= 0x05));
  assert(("Data is incorrect", memcmp(msg.data, message + 2, msg.length) == 0));

  printf("Message parsed successfully\n");
}
