; Auto-generated. Do not edit!


(cl:in-package RobotCar-srv)


;//! \htmlinclude web2robot-request.msg.html

(cl:defclass <web2robot-request> (roslisp-msg-protocol:ros-message)
  ((room_point
    :reader room_point
    :initarg :room_point
    :type cl:integer
    :initform 0)
   (robot_start
    :reader robot_start
    :initarg :robot_start
    :type cl:integer
    :initform 0))
)

(cl:defclass web2robot-request (<web2robot-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <web2robot-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'web2robot-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name RobotCar-srv:<web2robot-request> is deprecated: use RobotCar-srv:web2robot-request instead.")))

(cl:ensure-generic-function 'room_point-val :lambda-list '(m))
(cl:defmethod room_point-val ((m <web2robot-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader RobotCar-srv:room_point-val is deprecated.  Use RobotCar-srv:room_point instead.")
  (room_point m))

(cl:ensure-generic-function 'robot_start-val :lambda-list '(m))
(cl:defmethod robot_start-val ((m <web2robot-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader RobotCar-srv:robot_start-val is deprecated.  Use RobotCar-srv:robot_start instead.")
  (robot_start m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <web2robot-request>) ostream)
  "Serializes a message object of type '<web2robot-request>"
  (cl:let* ((signed (cl:slot-value msg 'room_point)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'robot_start)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <web2robot-request>) istream)
  "Deserializes a message object of type '<web2robot-request>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'room_point) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'robot_start) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<web2robot-request>)))
  "Returns string type for a service object of type '<web2robot-request>"
  "RobotCar/web2robotRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'web2robot-request)))
  "Returns string type for a service object of type 'web2robot-request"
  "RobotCar/web2robotRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<web2robot-request>)))
  "Returns md5sum for a message object of type '<web2robot-request>"
  "55c9a4696314bb0415612ef5211f4fc3")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'web2robot-request)))
  "Returns md5sum for a message object of type 'web2robot-request"
  "55c9a4696314bb0415612ef5211f4fc3")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<web2robot-request>)))
  "Returns full string definition for message of type '<web2robot-request>"
  (cl:format cl:nil "int32 room_point~%int32 robot_start~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'web2robot-request)))
  "Returns full string definition for message of type 'web2robot-request"
  (cl:format cl:nil "int32 room_point~%int32 robot_start~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <web2robot-request>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <web2robot-request>))
  "Converts a ROS message object to a list"
  (cl:list 'web2robot-request
    (cl:cons ':room_point (room_point msg))
    (cl:cons ':robot_start (robot_start msg))
))
;//! \htmlinclude web2robot-response.msg.html

(cl:defclass <web2robot-response> (roslisp-msg-protocol:ros-message)
  ((goal_point
    :reader goal_point
    :initarg :goal_point
    :type cl:integer
    :initform 0))
)

(cl:defclass web2robot-response (<web2robot-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <web2robot-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'web2robot-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name RobotCar-srv:<web2robot-response> is deprecated: use RobotCar-srv:web2robot-response instead.")))

(cl:ensure-generic-function 'goal_point-val :lambda-list '(m))
(cl:defmethod goal_point-val ((m <web2robot-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader RobotCar-srv:goal_point-val is deprecated.  Use RobotCar-srv:goal_point instead.")
  (goal_point m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <web2robot-response>) ostream)
  "Serializes a message object of type '<web2robot-response>"
  (cl:let* ((signed (cl:slot-value msg 'goal_point)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <web2robot-response>) istream)
  "Deserializes a message object of type '<web2robot-response>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'goal_point) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<web2robot-response>)))
  "Returns string type for a service object of type '<web2robot-response>"
  "RobotCar/web2robotResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'web2robot-response)))
  "Returns string type for a service object of type 'web2robot-response"
  "RobotCar/web2robotResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<web2robot-response>)))
  "Returns md5sum for a message object of type '<web2robot-response>"
  "55c9a4696314bb0415612ef5211f4fc3")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'web2robot-response)))
  "Returns md5sum for a message object of type 'web2robot-response"
  "55c9a4696314bb0415612ef5211f4fc3")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<web2robot-response>)))
  "Returns full string definition for message of type '<web2robot-response>"
  (cl:format cl:nil "int32 goal_point~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'web2robot-response)))
  "Returns full string definition for message of type 'web2robot-response"
  (cl:format cl:nil "int32 goal_point~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <web2robot-response>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <web2robot-response>))
  "Converts a ROS message object to a list"
  (cl:list 'web2robot-response
    (cl:cons ':goal_point (goal_point msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'web2robot)))
  'web2robot-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'web2robot)))
  'web2robot-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'web2robot)))
  "Returns string type for a service object of type '<web2robot>"
  "RobotCar/web2robot")