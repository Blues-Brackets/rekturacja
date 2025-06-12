#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

struct message
{
  uint8_t id;
  uint8_t length;
  uint8_t data[8];
};

int parse_message(uint8_t *message, int length, struct message *msg)
{
  int i;
  msg->id = message[0];
  msg->length = message[1];
  for (i = 0; i < msg->length; i++)
  {
    msg->data[i] = message[2 + i];
  }
  return 0;
}

struct another_message
{
  uint8_t id;
  uint16_t temperature;
  uint16_t humidity;
};

int parse_another_message(uint8_t *message, int length, struct another_message *msg)
{
  int i;
  msg->id = message[0];
  msg->temperature = message[1] << 8 | message[2];
  msg->humidity = message[3] << 8 | message[4];
  return 0;
}

int main()
{
  uint8_t message[] = {0x01, 0x05, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
  struct message msg;

  int ret = parse_message(message, sizeof(message), &msg);

  assert(("Failed to parse message", ret == 0));
  assert(("ID is incorrect", msg.id == 0x01));
  assert(("Length is incorrect", msg.length <= 0x05));
  assert(("Data is incorrect", memcmp(msg.data, message + 2, msg.length) == 0));

  uint8_t another_message[] = {0x01, 0x02, 0x03, 0x04, 0x05};
  struct another_message another_msg;

  int another_ret = parse_another_message(another_message, sizeof(another_message), &another_msg);

  assert(("Failed to parse another message", another_ret == 0));
  assert(("ID is incorrect", another_msg.id == 0x01));
  assert(("Temperature is incorrect", another_msg.temperature == 0x0203));
  assert(("Humidity is incorrect", another_msg.humidity == 0x0405));

  printf("Message parsed successfully\n");
}
