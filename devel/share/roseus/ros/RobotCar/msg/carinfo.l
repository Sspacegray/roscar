;; Auto-generated. Do not edit!


(when (boundp 'RobotCar::carinfo)
  (if (not (find-package "ROBOTCAR"))
    (make-package "ROBOTCAR"))
  (shadow 'carinfo (find-package "ROBOTCAR")))
(unless (find-package "ROBOTCAR::CARINFO")
  (make-package "ROBOTCAR::CARINFO"))

(in-package "ROS")
;;//! \htmlinclude carinfo.msg.html


(defclass RobotCar::carinfo
  :super ros::object
  :slots (_speed_x _speed_z _power _limit_down _limit_up _goal_point _robot_start ))

(defmethod RobotCar::carinfo
  (:init
   (&key
    ((:speed_x __speed_x) 0)
    ((:speed_z __speed_z) 0)
    ((:power __power) 0)
    ((:limit_down __limit_down) 0)
    ((:limit_up __limit_up) 0)
    ((:goal_point __goal_point) 0)
    ((:robot_start __robot_start) 0)
    )
   (send-super :init)
   (setq _speed_x (round __speed_x))
   (setq _speed_z (round __speed_z))
   (setq _power (round __power))
   (setq _limit_down (round __limit_down))
   (setq _limit_up (round __limit_up))
   (setq _goal_point (round __goal_point))
   (setq _robot_start (round __robot_start))
   self)
  (:speed_x
   (&optional __speed_x)
   (if __speed_x (setq _speed_x __speed_x)) _speed_x)
  (:speed_z
   (&optional __speed_z)
   (if __speed_z (setq _speed_z __speed_z)) _speed_z)
  (:power
   (&optional __power)
   (if __power (setq _power __power)) _power)
  (:limit_down
   (&optional __limit_down)
   (if __limit_down (setq _limit_down __limit_down)) _limit_down)
  (:limit_up
   (&optional __limit_up)
   (if __limit_up (setq _limit_up __limit_up)) _limit_up)
  (:goal_point
   (&optional __goal_point)
   (if __goal_point (setq _goal_point __goal_point)) _goal_point)
  (:robot_start
   (&optional __robot_start)
   (if __robot_start (setq _robot_start __robot_start)) _robot_start)
  (:serialization-length
   ()
   (+
    ;; int32 _speed_x
    4
    ;; int32 _speed_z
    4
    ;; int32 _power
    4
    ;; int32 _limit_down
    4
    ;; int32 _limit_up
    4
    ;; int32 _goal_point
    4
    ;; int32 _robot_start
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _speed_x
       (write-long _speed_x s)
     ;; int32 _speed_z
       (write-long _speed_z s)
     ;; int32 _power
       (write-long _power s)
     ;; int32 _limit_down
       (write-long _limit_down s)
     ;; int32 _limit_up
       (write-long _limit_up s)
     ;; int32 _goal_point
       (write-long _goal_point s)
     ;; int32 _robot_start
       (write-long _robot_start s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _speed_x
     (setq _speed_x (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _speed_z
     (setq _speed_z (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _power
     (setq _power (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _limit_down
     (setq _limit_down (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _limit_up
     (setq _limit_up (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _goal_point
     (setq _goal_point (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _robot_start
     (setq _robot_start (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get RobotCar::carinfo :md5sum-) "0928fc105e49385f0a0196c2865ce602")
(setf (get RobotCar::carinfo :datatype-) "RobotCar/carinfo")
(setf (get RobotCar::carinfo :definition-)
      "int32 speed_x
int32 speed_z
int32 power
int32 limit_down
int32 limit_up
int32 goal_point
int32 robot_start
")



(provide :RobotCar/carinfo "0928fc105e49385f0a0196c2865ce602")


