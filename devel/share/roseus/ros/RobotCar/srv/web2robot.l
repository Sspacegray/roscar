;; Auto-generated. Do not edit!


(when (boundp 'RobotCar::web2robot)
  (if (not (find-package "ROBOTCAR"))
    (make-package "ROBOTCAR"))
  (shadow 'web2robot (find-package "ROBOTCAR")))
(unless (find-package "ROBOTCAR::WEB2ROBOT")
  (make-package "ROBOTCAR::WEB2ROBOT"))
(unless (find-package "ROBOTCAR::WEB2ROBOTREQUEST")
  (make-package "ROBOTCAR::WEB2ROBOTREQUEST"))
(unless (find-package "ROBOTCAR::WEB2ROBOTRESPONSE")
  (make-package "ROBOTCAR::WEB2ROBOTRESPONSE"))

(in-package "ROS")





(defclass RobotCar::web2robotRequest
  :super ros::object
  :slots (_room_point _robot_start ))

(defmethod RobotCar::web2robotRequest
  (:init
   (&key
    ((:room_point __room_point) 0)
    ((:robot_start __robot_start) 0)
    )
   (send-super :init)
   (setq _room_point (round __room_point))
   (setq _robot_start (round __robot_start))
   self)
  (:room_point
   (&optional __room_point)
   (if __room_point (setq _room_point __room_point)) _room_point)
  (:robot_start
   (&optional __robot_start)
   (if __robot_start (setq _robot_start __robot_start)) _robot_start)
  (:serialization-length
   ()
   (+
    ;; int32 _room_point
    4
    ;; int32 _robot_start
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _room_point
       (write-long _room_point s)
     ;; int32 _robot_start
       (write-long _robot_start s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _room_point
     (setq _room_point (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _robot_start
     (setq _robot_start (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(defclass RobotCar::web2robotResponse
  :super ros::object
  :slots (_goal_point ))

(defmethod RobotCar::web2robotResponse
  (:init
   (&key
    ((:goal_point __goal_point) 0)
    )
   (send-super :init)
   (setq _goal_point (round __goal_point))
   self)
  (:goal_point
   (&optional __goal_point)
   (if __goal_point (setq _goal_point __goal_point)) _goal_point)
  (:serialization-length
   ()
   (+
    ;; int32 _goal_point
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _goal_point
       (write-long _goal_point s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _goal_point
     (setq _goal_point (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(defclass RobotCar::web2robot
  :super ros::object
  :slots ())

(setf (get RobotCar::web2robot :md5sum-) "55c9a4696314bb0415612ef5211f4fc3")
(setf (get RobotCar::web2robot :datatype-) "RobotCar/web2robot")
(setf (get RobotCar::web2robot :request) RobotCar::web2robotRequest)
(setf (get RobotCar::web2robot :response) RobotCar::web2robotResponse)

(defmethod RobotCar::web2robotRequest
  (:response () (instance RobotCar::web2robotResponse :init)))

(setf (get RobotCar::web2robotRequest :md5sum-) "55c9a4696314bb0415612ef5211f4fc3")
(setf (get RobotCar::web2robotRequest :datatype-) "RobotCar/web2robotRequest")
(setf (get RobotCar::web2robotRequest :definition-)
      "int32 room_point
int32 robot_start

---
int32 goal_point
")

(setf (get RobotCar::web2robotResponse :md5sum-) "55c9a4696314bb0415612ef5211f4fc3")
(setf (get RobotCar::web2robotResponse :datatype-) "RobotCar/web2robotResponse")
(setf (get RobotCar::web2robotResponse :definition-)
      "int32 room_point
int32 robot_start

---
int32 goal_point
")



(provide :RobotCar/web2robot "55c9a4696314bb0415612ef5211f4fc3")


