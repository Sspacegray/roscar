// Auto-generated. Do not edit!

// (in-package RobotCar.srv)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------


//-----------------------------------------------------------

class web2robotRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.room_point = null;
      this.robot_start = null;
    }
    else {
      if (initObj.hasOwnProperty('room_point')) {
        this.room_point = initObj.room_point
      }
      else {
        this.room_point = 0;
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
    // Serializes a message object of type web2robotRequest
    // Serialize message field [room_point]
    bufferOffset = _serializer.int32(obj.room_point, buffer, bufferOffset);
    // Serialize message field [robot_start]
    bufferOffset = _serializer.int32(obj.robot_start, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type web2robotRequest
    let len;
    let data = new web2robotRequest(null);
    // Deserialize message field [room_point]
    data.room_point = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [robot_start]
    data.robot_start = _deserializer.int32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 8;
  }

  static datatype() {
    // Returns string type for a service object
    return 'RobotCar/web2robotRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '870de0baf0d45273772e8d7e3c64ba59';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int32 room_point
    int32 robot_start
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new web2robotRequest(null);
    if (msg.room_point !== undefined) {
      resolved.room_point = msg.room_point;
    }
    else {
      resolved.room_point = 0
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

class web2robotResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.goal_point = null;
    }
    else {
      if (initObj.hasOwnProperty('goal_point')) {
        this.goal_point = initObj.goal_point
      }
      else {
        this.goal_point = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type web2robotResponse
    // Serialize message field [goal_point]
    bufferOffset = _serializer.int32(obj.goal_point, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type web2robotResponse
    let len;
    let data = new web2robotResponse(null);
    // Deserialize message field [goal_point]
    data.goal_point = _deserializer.int32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 4;
  }

  static datatype() {
    // Returns string type for a service object
    return 'RobotCar/web2robotResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '836a7491234659f67cc4e1bb62045b48';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int32 goal_point
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new web2robotResponse(null);
    if (msg.goal_point !== undefined) {
      resolved.goal_point = msg.goal_point;
    }
    else {
      resolved.goal_point = 0
    }

    return resolved;
    }
};

module.exports = {
  Request: web2robotRequest,
  Response: web2robotResponse,
  md5sum() { return '55c9a4696314bb0415612ef5211f4fc3'; },
  datatype() { return 'RobotCar/web2robot'; }
};
