#pragma once
// MESSAGE CHASSIS PACKING

#define MAVLINK_MSG_ID_CHASSIS 1


typedef struct __mavlink_chassis_t {
 double radius; /*<  radius of weight*/
 float pos_x; /*<  x Position*/
 float pos_y; /*<  x Position*/
 float theta; /*<  Heading angle*/
 uint8_t state; /*<  state machine*/
 uint8_t flag; /*<  left or right*/
} mavlink_chassis_t;

#define MAVLINK_MSG_ID_CHASSIS_LEN 22
#define MAVLINK_MSG_ID_CHASSIS_MIN_LEN 22
#define MAVLINK_MSG_ID_1_LEN 22
#define MAVLINK_MSG_ID_1_MIN_LEN 22

#define MAVLINK_MSG_ID_CHASSIS_CRC 116
#define MAVLINK_MSG_ID_1_CRC 116



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_CHASSIS { \
    1, \
    "CHASSIS", \
    6, \
    {  { "pos_x", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_chassis_t, pos_x) }, \
         { "pos_y", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_chassis_t, pos_y) }, \
         { "radius", NULL, MAVLINK_TYPE_DOUBLE, 0, 0, offsetof(mavlink_chassis_t, radius) }, \
         { "theta", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_chassis_t, theta) }, \
         { "state", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_chassis_t, state) }, \
         { "flag", NULL, MAVLINK_TYPE_UINT8_T, 0, 21, offsetof(mavlink_chassis_t, flag) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_CHASSIS { \
    "CHASSIS", \
    6, \
    {  { "pos_x", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_chassis_t, pos_x) }, \
         { "pos_y", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_chassis_t, pos_y) }, \
         { "radius", NULL, MAVLINK_TYPE_DOUBLE, 0, 0, offsetof(mavlink_chassis_t, radius) }, \
         { "theta", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_chassis_t, theta) }, \
         { "state", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_chassis_t, state) }, \
         { "flag", NULL, MAVLINK_TYPE_UINT8_T, 0, 21, offsetof(mavlink_chassis_t, flag) }, \
         } \
}
#endif

/**
 * @brief Pack a chassis message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param pos_x  x Position
 * @param pos_y  x Position
 * @param radius  radius of weight
 * @param theta  Heading angle
 * @param state  state machine
 * @param flag  left or right
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_chassis_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float pos_x, float pos_y, double radius, float theta, uint8_t state, uint8_t flag)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CHASSIS_LEN];
    _mav_put_double(buf, 0, radius);
    _mav_put_float(buf, 8, pos_x);
    _mav_put_float(buf, 12, pos_y);
    _mav_put_float(buf, 16, theta);
    _mav_put_uint8_t(buf, 20, state);
    _mav_put_uint8_t(buf, 21, flag);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CHASSIS_LEN);
#else
    mavlink_chassis_t packet;
    packet.radius = radius;
    packet.pos_x = pos_x;
    packet.pos_y = pos_y;
    packet.theta = theta;
    packet.state = state;
    packet.flag = flag;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CHASSIS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CHASSIS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CHASSIS_MIN_LEN, MAVLINK_MSG_ID_CHASSIS_LEN, MAVLINK_MSG_ID_CHASSIS_CRC);
}

/**
 * @brief Pack a chassis message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param pos_x  x Position
 * @param pos_y  x Position
 * @param radius  radius of weight
 * @param theta  Heading angle
 * @param state  state machine
 * @param flag  left or right
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_chassis_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               float pos_x, float pos_y, double radius, float theta, uint8_t state, uint8_t flag)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CHASSIS_LEN];
    _mav_put_double(buf, 0, radius);
    _mav_put_float(buf, 8, pos_x);
    _mav_put_float(buf, 12, pos_y);
    _mav_put_float(buf, 16, theta);
    _mav_put_uint8_t(buf, 20, state);
    _mav_put_uint8_t(buf, 21, flag);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CHASSIS_LEN);
#else
    mavlink_chassis_t packet;
    packet.radius = radius;
    packet.pos_x = pos_x;
    packet.pos_y = pos_y;
    packet.theta = theta;
    packet.state = state;
    packet.flag = flag;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CHASSIS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CHASSIS;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CHASSIS_MIN_LEN, MAVLINK_MSG_ID_CHASSIS_LEN, MAVLINK_MSG_ID_CHASSIS_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CHASSIS_MIN_LEN, MAVLINK_MSG_ID_CHASSIS_LEN);
#endif
}

/**
 * @brief Pack a chassis message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param pos_x  x Position
 * @param pos_y  x Position
 * @param radius  radius of weight
 * @param theta  Heading angle
 * @param state  state machine
 * @param flag  left or right
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_chassis_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float pos_x,float pos_y,double radius,float theta,uint8_t state,uint8_t flag)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CHASSIS_LEN];
    _mav_put_double(buf, 0, radius);
    _mav_put_float(buf, 8, pos_x);
    _mav_put_float(buf, 12, pos_y);
    _mav_put_float(buf, 16, theta);
    _mav_put_uint8_t(buf, 20, state);
    _mav_put_uint8_t(buf, 21, flag);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CHASSIS_LEN);
#else
    mavlink_chassis_t packet;
    packet.radius = radius;
    packet.pos_x = pos_x;
    packet.pos_y = pos_y;
    packet.theta = theta;
    packet.state = state;
    packet.flag = flag;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CHASSIS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CHASSIS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CHASSIS_MIN_LEN, MAVLINK_MSG_ID_CHASSIS_LEN, MAVLINK_MSG_ID_CHASSIS_CRC);
}

/**
 * @brief Encode a chassis struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param chassis C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_chassis_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_chassis_t* chassis)
{
    return mavlink_msg_chassis_pack(system_id, component_id, msg, chassis->pos_x, chassis->pos_y, chassis->radius, chassis->theta, chassis->state, chassis->flag);
}

/**
 * @brief Encode a chassis struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param chassis C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_chassis_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_chassis_t* chassis)
{
    return mavlink_msg_chassis_pack_chan(system_id, component_id, chan, msg, chassis->pos_x, chassis->pos_y, chassis->radius, chassis->theta, chassis->state, chassis->flag);
}

/**
 * @brief Encode a chassis struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param chassis C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_chassis_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_chassis_t* chassis)
{
    return mavlink_msg_chassis_pack_status(system_id, component_id, _status, msg,  chassis->pos_x, chassis->pos_y, chassis->radius, chassis->theta, chassis->state, chassis->flag);
}

/**
 * @brief Send a chassis message
 * @param chan MAVLink channel to send the message
 *
 * @param pos_x  x Position
 * @param pos_y  x Position
 * @param radius  radius of weight
 * @param theta  Heading angle
 * @param state  state machine
 * @param flag  left or right
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_chassis_send(mavlink_channel_t chan, float pos_x, float pos_y, double radius, float theta, uint8_t state, uint8_t flag)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CHASSIS_LEN];
    _mav_put_double(buf, 0, radius);
    _mav_put_float(buf, 8, pos_x);
    _mav_put_float(buf, 12, pos_y);
    _mav_put_float(buf, 16, theta);
    _mav_put_uint8_t(buf, 20, state);
    _mav_put_uint8_t(buf, 21, flag);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CHASSIS, buf, MAVLINK_MSG_ID_CHASSIS_MIN_LEN, MAVLINK_MSG_ID_CHASSIS_LEN, MAVLINK_MSG_ID_CHASSIS_CRC);
#else
    mavlink_chassis_t packet;
    packet.radius = radius;
    packet.pos_x = pos_x;
    packet.pos_y = pos_y;
    packet.theta = theta;
    packet.state = state;
    packet.flag = flag;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CHASSIS, (const char *)&packet, MAVLINK_MSG_ID_CHASSIS_MIN_LEN, MAVLINK_MSG_ID_CHASSIS_LEN, MAVLINK_MSG_ID_CHASSIS_CRC);
#endif
}

/**
 * @brief Send a chassis message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_chassis_send_struct(mavlink_channel_t chan, const mavlink_chassis_t* chassis)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_chassis_send(chan, chassis->pos_x, chassis->pos_y, chassis->radius, chassis->theta, chassis->state, chassis->flag);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CHASSIS, (const char *)chassis, MAVLINK_MSG_ID_CHASSIS_MIN_LEN, MAVLINK_MSG_ID_CHASSIS_LEN, MAVLINK_MSG_ID_CHASSIS_CRC);
#endif
}

#if MAVLINK_MSG_ID_CHASSIS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_chassis_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float pos_x, float pos_y, double radius, float theta, uint8_t state, uint8_t flag)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_double(buf, 0, radius);
    _mav_put_float(buf, 8, pos_x);
    _mav_put_float(buf, 12, pos_y);
    _mav_put_float(buf, 16, theta);
    _mav_put_uint8_t(buf, 20, state);
    _mav_put_uint8_t(buf, 21, flag);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CHASSIS, buf, MAVLINK_MSG_ID_CHASSIS_MIN_LEN, MAVLINK_MSG_ID_CHASSIS_LEN, MAVLINK_MSG_ID_CHASSIS_CRC);
#else
    mavlink_chassis_t *packet = (mavlink_chassis_t *)msgbuf;
    packet->radius = radius;
    packet->pos_x = pos_x;
    packet->pos_y = pos_y;
    packet->theta = theta;
    packet->state = state;
    packet->flag = flag;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CHASSIS, (const char *)packet, MAVLINK_MSG_ID_CHASSIS_MIN_LEN, MAVLINK_MSG_ID_CHASSIS_LEN, MAVLINK_MSG_ID_CHASSIS_CRC);
#endif
}
#endif

#endif

// MESSAGE CHASSIS UNPACKING


/**
 * @brief Get field pos_x from chassis message
 *
 * @return  x Position
 */
static inline float mavlink_msg_chassis_get_pos_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field pos_y from chassis message
 *
 * @return  x Position
 */
static inline float mavlink_msg_chassis_get_pos_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field radius from chassis message
 *
 * @return  radius of weight
 */
static inline double mavlink_msg_chassis_get_radius(const mavlink_message_t* msg)
{
    return _MAV_RETURN_double(msg,  0);
}

/**
 * @brief Get field theta from chassis message
 *
 * @return  Heading angle
 */
static inline float mavlink_msg_chassis_get_theta(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field state from chassis message
 *
 * @return  state machine
 */
static inline uint8_t mavlink_msg_chassis_get_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  20);
}

/**
 * @brief Get field flag from chassis message
 *
 * @return  left or right
 */
static inline uint8_t mavlink_msg_chassis_get_flag(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  21);
}

/**
 * @brief Decode a chassis message into a struct
 *
 * @param msg The message to decode
 * @param chassis C-struct to decode the message contents into
 */
static inline void mavlink_msg_chassis_decode(const mavlink_message_t* msg, mavlink_chassis_t* chassis)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    chassis->radius = mavlink_msg_chassis_get_radius(msg);
    chassis->pos_x = mavlink_msg_chassis_get_pos_x(msg);
    chassis->pos_y = mavlink_msg_chassis_get_pos_y(msg);
    chassis->theta = mavlink_msg_chassis_get_theta(msg);
    chassis->state = mavlink_msg_chassis_get_state(msg);
    chassis->flag = mavlink_msg_chassis_get_flag(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_CHASSIS_LEN? msg->len : MAVLINK_MSG_ID_CHASSIS_LEN;
        memset(chassis, 0, MAVLINK_MSG_ID_CHASSIS_LEN);
    memcpy(chassis, _MAV_PAYLOAD(msg), len);
#endif
}
