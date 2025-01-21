; Auto-generated. Do not edit!


(cl:in-package RobotCar-msg)


;//! \htmlinclude carinfo.msg.html

(cl:defclass <carinfo> (roslisp-msg-protocol:ros-message)
  ((speed_x
    :reader speed_x
    :initarg :speed_x
    :type cl:integer
    :initform 0)
   (speed_z
    :reader speed_z
    :initarg :speed_z
    :type cl:integer
    :initform 0)
   (power
    :reader power
    :initarg :power
    :type cl:integer
    :initform 0)
   (limit_down
    :reader limit_down
    :initarg :limit_down
    :type cl:integer
    :initform 0)
   (limit_up
    :reader limit_up
    :initarg :limit_up
    :type cl:integer
    :initform 0)
   (goal_point
    :reader goal_point
    :initarg :goal_point
    :type cl:integer
    :initform 0)
   (robot_start
    :reader robot_start
    :initarg :robot_start
    :type cl:integer
    :initform 0))
)

(cl:defclass carinfo (<carinfo>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <carinfo>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'carinfo)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name RobotCar-msg:<carinfo> is deprecated: use RobotCar-msg:carinfo instead.")))

(cl:ensure-generic-function 'speed_x-val :lambda-list '(m))
(cl:defmethod speed_x-val ((m <carinfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader RobotCar-msg:speed_x-val is deprecated.  Use RobotCar-msg:speed_x instead.")
  (speed_x m))

(cl:ensure-generic-function 'speed_z-val :lambda-list '(m))
(cl:defmethod speed_z-val ((m <carinfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader RobotCar-msg:speed_z-val is deprecated.  Use RobotCar-msg:speed_z instead.")
  (speed_z m))

(cl:ensure-generic-function 'power-val :lambda-list '(m))
(cl:defmethod power-val ((m <carinfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader RobotCar-msg:power-val is deprecated.  Use RobotCar-msg:power instead.")
  (power m))

(cl:ensure-generic-function 'limit_down-val :lambda-list '(m))
(cl:defmethod limit_down-val ((m <carinfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader RobotCar-msg:limit_down-val is deprecated.  Use RobotCar-msg:limit_down instead.")
  (limit_down m))

(cl:ensure-generic-function 'limit_up-val :lambda-list '(m))
(cl:defmethod limit_up-val ((m <carinfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader RobotCar-msg:limit_up-val is deprecated.  Use RobotCar-msg:limit_up instead.")
  (limit_up m))

(cl:ensure-generic-function 'goal_point-val :lambda-list '(m))
(cl:defmethod goal_point-val ((m <carinfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader RobotCar-msg:goal_point-val is deprecated.  Use RobotCar-msg:goal_point instead.")
  (goal_point m))

(cl:ensure-generic-function 'robot_start-val :lambda-list '(m))
(cl:defmethod robot_start-val ((m <carinfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader RobotCar-msg:robot_start-val is deprecated.  Use RobotCar-msg:robot_start instead.")
  (robot_start m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <carinfo>) ostream)
  "Serializes a message object of type '<carinfo>"
  (cl:let* ((signed (cl:slot-value msg 'speed_x)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'speed_z)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'power)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'limit_down)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'limit_up)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'goal_point)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
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
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <carinfo>) istream)
  "Deserializes a message object of type '<carinfo>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'speed_x) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'speed_z) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'power) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'limit_down) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'limit_up) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'goal_point) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'robot_start) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<carinfo>)))
  "Returns string type for a message object of type '<carinfo>"
  "RobotCar/carinfo")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'carinfo)))
  "Returns string type for a message object of type 'carinfo"
  "RobotCar/carinfo")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<carinfo>)))
  "Returns md5sum for a message object of type '<carinfo>"
  "0928fc105e49385f0a0196c2865ce602")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'carinfo)))
  "Returns md5sum for a message object of type 'carinfo"
  "0928fc105e49385f0a0196c2865ce602")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<carinfo>)))
  "Returns full string definition for message of type '<carinfo>"
  (cl:format cl:nil "int32 speed_x~%int32 speed_z~%int32 power~%int32 limit_down~%int32 limit_up~%int32 goal_point~%int32 robot_start~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'carinfo)))
  "Returns full string definition for message of type 'carinfo"
  (cl:format cl:nil "int32 speed_x~%int32 speed_z~%int32 power~%int32 limit_down~%int32 limit_up~%int32 goal_point~%int32 robot_start~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <carinfo>))
  (cl:+ 0
     4
     4
     4
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <carinfo>))
  "Converts a ROS message object to a list"
  (cl:list 'carinfo
    (cl:cons ':speed_x (speed_x msg))
    (cl:cons ':speed_z (speed_z msg))
    (cl:cons ':power (power msg))
    (cl:cons ':limit_down (limit_down msg))
    (cl:cons ':limit_up (limit_up msg))
    (cl:cons ':goal_point (goal_point msg))
    (cl:cons ':robot_start (robot_start msg))
))
