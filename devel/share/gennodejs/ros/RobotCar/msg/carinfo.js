// Auto-generated. Do not edit!

// (in-package RobotCar.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class carinfo {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.speed_x = null;
      this.speed_z = null;
      this.power = null;
      this.limit_down = null;
      this.limit_up = null;
      this.goal_point = null;
      this.robot_start = null;
    }
    else {
      if (initObj.hasOwnProperty('speed_x')) {
        this.speed_x = initObj.speed_x
      }
      else {
        this.speed_x = 0;
      }
      if (initObj.hasOwnProperty('speed_z')) {
        this.speed_z = initObj.speed_z
      }
      else {
        this.speed_z = 0;
      }
      if (initObj.hasOwnProperty('power')) {
        this.power = initObj.power
      }
      else {
        this.power = 0;
      }
      if (initObj.hasOwnProperty('limit_down')) {
        this.limit_down = initObj.limit_down
      }
      else {
        this.limit_down = 0;
      }
      if (initObj.hasOwnProperty('limit_up')) {
        this.limit_up = initObj.limit_up
      }
      else {
        this.limit_up = 0;
      }
      if (initObj.hasOwnProperty('goal_point')) {
        this.goal_point = initObj.goal_point
      }
      else {
        this.goal_point = 0;
      }
      if (initObj.hasOwnProperty('robot_start')) {
        this.robot_start = initObj.robot_start
      }
      else {
        this.robot_start = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type carinfo
    // Serialize message field [speed_x]
    bufferOffset = _serializer.int32(obj.speed_x, buffer, bufferOffset);
    // Serialize message field [speed_z]
    bufferOffset = _serializer.int32(obj.speed_z, buffer, bufferOffset);
    // Serialize message field [power]
    bufferOffset = _serializer.int32(obj.power, buffer, bufferOffset);
    // Serialize message field [limit_down]
    bufferOffset = _serializer.int32(obj.limit_down, buffer, bufferOffset);
    // Serialize message field [limit_up]
    bufferOffset = _serializer.int32(obj.limit_up, buffer, bufferOffset);
    // Serialize message field [goal_point]
    bufferOffset = _serializer.int32(obj.goal_point, buffer, bufferOffset);
    // Serialize message field [robot_start]
    bufferOffset = _serializer.int32(obj.robot_start, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type carinfo
    let len;
    let data = new carinfo(null);
    // Deserialize message field [speed_x]
    data.speed_x = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [speed_z]
    data.speed_z = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [power]
    data.power = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [limit_down]
    data.limit_down = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [limit_up]
    data.limit_up = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [goal_point]
    data.goal_point = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [robot_start]
    data.robot_start = _deserializer.int32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 28;
  }

  static datatype() {
    // Returns string type for a message object
    return 'RobotCar/carinfo';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '0928fc105e49385f0a0196c2865ce602';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int32 speed_x
    int32 speed_z
    int32 power
    int32 limit_down
    int32 limit_up
    int32 goal_point
    int32 robot_start
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new carinfo(null);
    if (msg.speed_x !== undefined) {
      resolved.speed_x = msg.speed_x;
    }
    else {
      resolved.speed_x = 0
    }

    if (msg.speed_z !== undefined) {
      resolved.speed_z = msg.speed_z;
    }
    else {
      resolved.speed_z = 0
    }

    if (msg.power !== undefined) {
      resolved.power = msg.power;
    }
    else {
      resolved.power = 0
    }

    if (msg.limit_down !== undefined) {
      resolved.limit_down = msg.limit_down;
    }
    else {
      resolved.limit_down = 0
    }

    if (msg.limit_up !== undefined) {
      resolved.limit_up = msg.limit_up;
    }
    else {
      resolved.limit_up = 0
    }

    if (msg.goal_point !== undefined) {
      resolved.goal_point = msg.goal_point;
    }
    else {
      resolved.goal_point = 0
    }

    if (msg.robot_start !== undefined) {
      resolved.robot_start = msg.robot_start;
    }
    else {
      resolved.robot_start = 0
    }

    return resolved;
    }
};

module.exports = carinfo;
