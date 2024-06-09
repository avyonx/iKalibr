<div align=center><img src="../img/logo.svg" width =100%></div>

---

<h3 align="center">Configure File Format In iKalibr</h3>
<p align="center">
    <a href="https://github.com/Unsigned-Long"><strong>Author » Shuolong Chen</strong></a>
</p>

---

Like it or not, let’s just throw out a typical configuration file with detailed comments as follows. In fact, we believe that you are able to modify it to adapt your sensor suite by yourself after reading this. 

**Tips**: don't be intimidated by the length of this configuration file. **It is long since it contains detailed comments to help you understand it.** As the saying goes: **more is less!** It's easy to understand, I promise! Most of the time, you only need to modify a few items to switch to another sensor suite to calibrate.

After reading this, perhaps the left questions are about how to choose the sensor type, see [here](sensor_type.md) for more information.

```yaml
Configor:
  DataStream:
    # key: IMU topic, value: IMU type. Supported IMU types are:
    #   1. SENSOR_IMU: https://docs.ros.org/en/noetic/api/sensor_msgs/html/msg/Imu.html (acce unit: m/s^2)
    #   2. SBG_IMU: https://github.com/SBG-Systems/sbg_ros_driver.git (acce unit: m/s^2)
    #   3. SENSOR_IMU_G: https://docs.ros.org/en/noetic/api/sensor_msgs/html/msg/Imu.html (acce unit: G)
    #   4. ...
    IMUTopics:
      - key: "/imu0/frame"
        value:
          Type: "SENSOR_IMU"
          # the inertial intrinsic filename, a default template (identity) is provided in 'ikalibr/config'
          # if you want to perform IMU-only multi-IMU calibration, you are expected to pre-calibrate
          # the intrinics of your IMUs using program 'ikalibr_imu_intri_calib'. Otherwise, just use the default
          # one, since for other sensor suites, such as visual-inertial, radar-inertial, etc., 
          # inertial intrinsics would be optimized in spatiotemporal calibration
          Intrinsics: "/home/csl/ros_ws/iKalibr/src/ikalibr/config/imu-intri.yaml"
          # the weights should be set based on the noise level of your own IMUs
          # large noise level points to small weight
          AcceWeight: 17.68
          GyroWeight: 57.66
      # the second IMU, if only one IMU is integrated, just comment out the following key item
      - key: "/imu1/frame"
        value:
          Type: "SBG_IMU"
          Intrinsics: "/home/csl/ros_ws/iKalibr/src/ikalibr/config/imu-intri.yaml"
          AcceWeight: 17.68
          GyroWeight: 57.66
    # key: radar topic, value: radar type. Supported radar types are:
    #   1.         AINSTEIN_RADAR: https://github.com/AinsteinAI/ainstein_radar.git
    #   2.       AWR1843BOOST_RAW: https://github.com/Unsigned-Long/ti_mmwave_rospkg.git
    #   3.    AWR1843BOOST_CUSTOM: https://github.com/Unsigned-Long/ti_mmwave_rospkg.git
    #   4.       POINTCLOUD2_POSV: 'sensor_msgs/PointCloud2' with point format: [x, y, z, velocity]
    #   5.      POINTCLOUD2_POSIV: 'sensor_msgs/PointCloud2' with point format: [x, y, z, intensity, velocity]
    #   6.       POINTCLOUD2_XRIO: https://github.com/christopherdoer/rio.git
    #   7. ...
    RadarTopics:
      # if no radar is integrated in your sensor suite, just comment out the following key items
      - key: "/radar0/scan"
        value:
          Type: "AWR1843BOOST_CUSTOM"
          Weight: 10.0
      - key: "/radar1/scan"
        value:
          Type: "AWR1843BOOST_CUSTOM"
          Weight: 10.0
    # key: LiDAR topic, value: LiDAR type. Supported LiDAR types:
    #   1.        Velodyne LiDARs: VLP_16_PACKET, VLP_16_POINTS, VLP_32E_POINTS
    #   2.          Ouster LiDARs: OUSTER_16_POINTS OUSTER_32_POINTS, OUSTER_64_POINTS, OUSTER_128_POINTS
    #   3. Hesai Pandar XT LiDARs: PANDAR_XT_16, PANDAR_XT_32
    #   4.           Livox LiDARs: LIVOX_MID_360, LIVOX_AVIA
    LiDARTopics:
      # if no LiDAR is integrated in your sensor suite, just comment out the following key items
      - key: "/lidar0/scan"
        value:
          Type: "VLP_32E_POINTS"
          Weight: 100.0
      - key: "/lidar1/scan"
        value:
          Type: "LIVOX_AVIA"
          Weight: 80.0
    # key: camera topic, value: camera type. Supported camera types:
    #   1. SENSOR_IMAGE_GS: https://docs.ros.org/en/noetic/api/sensor_msgs/html/msg/Image.html
    #   2. SENSOR_IMAGE_RS: https://docs.ros.org/en/noetic/api/sensor_msgs/html/msg/Image.html
    CameraTopics:
      # if no camera is integrated in your sensor suite, just comment out the following key items
      - key: "/camera0/frame"
        value:
          Type: "SENSOR_IMAGE_GS"
          # the camera intrinsic filename, default templates are provided in 'ikalibr/config'
          # two types of optical cameras are support, namely 'pinhole-brown' and 'pinhole-fisheye'
          # these camera intrinics are required pre-calibrated
          # Kalibr is recommend to perform intrinsic calibration (https://github.com/ethz-asl/kalibr.git)
          Intrinsics: "/home/csl/ros_ws/iKalibr/src/ikalibr/.../cam0-intri.yaml"
          Weight: 50.0
          # use visual features that are tracked larger than 'TrackLengthMin'
          # smaller value means less features are considered in solving
          TrackLengthMin: 10
      - key: "/camera1/frame"
        value:
          Type: "SENSOR_IMAGE_RS"
          Intrinsics: "/home/csl/ros_ws/iKalibr/src/ikalibr/.../cam1-intri.yaml"
          Weight: 50.0
          TrackLengthMin: 10
    # the reference IMU, it should be one of multiple IMUs (the ros topic of the IMU)
    ReferIMU: "/imu1/frame"
    BagPath: "/home/csl/dataset/.../imus_radars_lidars_cameras.bag"
    # the time piece: [BegTime, BegTime + Duration], unit: second(s)
    # if you want to use all time data for calibration, please set them to negative numbers
    # Note that the 'BegTime' here is measured from the start time of bag
    # and is not related to the timestamp of the data recorded in the package.
    # an example where multi-sensor data in time range [40, 80] is connected to excited motion
    BeginTime: 40
    Duration: 40
    OutputPath: "/home/csl/ros_ws/iKalibr/.../ws"
  Prior:
    # unit: m/s^2
    GravityNorm: 9.8
    # if sensor are hardware-synchronized, you could choose to fix temporal parameters 
    # by setting this field to 'false'
    OptTemporalParams: true
    # the range where the time offsets would be optimized.
    # make sure this range contains the ground truth of time offsets
    # If you're not sure, make this field large, but this could lead to longer optimization time
    TimeOffsetPadding: 0.10
    # readout time padding for RS camera, make sure this range contains the ground truth
    ReadoutTimePadding: 0.01
    # the time distance of two neighbor control points, which determines the accuracy
    # of the representation of the B-splines. Smaller distance would lead to longer optimization time
    # common choices: from '0.01' to '0.10'
    KnotTimeDist:
      SO3Spline: 0.05
      ScaleSpline: 0.05
    # when lidar is involved in the calibration framework, the ndt odometer is employed to recover pose roughly
    NDTLiDAROdometer:
      # 0.5 for indoor case and 1.0 for outdoor case
      Resolution: 0.5
      KeyFrameDownSample: 0.1
    LiDARDataAssociate:
      # leaf size when down sample the map using 'pcl::VoxelGrid' filter
      # note that this field just for visualization, no connection with calibration
      # for outdoor, 0.05 is suggested, and for indoor: 0.1 is suggested
      MapDownSample: 0.05
      # associate point and surfel when distance is less than this value
      PointToSurfelMax: 0.1
      # chose plane as a surfel for data association when planarity is larger than this value
      PlanarityMin: 0.6
    # the loss function used for radar factor (m/s)
    CauchyLossForRadarFactor: 0.1
    # the loss function used for lidar factor (m)
    CauchyLossForLiDARFactor: 0.5
    # the loss function used for visual reprojection factor (pixel)
    CauchyLossForCameraFactor: 1.0
  Preference:
    # whether using cuda to speed up when solving least-squares problems
    # if you do not install the cuda dependency, set it to 'false'
    UseCudaInSolving: false
    # whether output the spatiotemporal parameters each iteration
    OutputParamInEachIter: true
    # whether output B-splines
    OutputBSplines: true
    # whether output maps
    OutputMaps: true
    # whether output hessian matrix
    OutputHessianMat: true
    # whether covisibility image between camera and lidar
    OutputVisualLiDARCovisibility: true
    # whether output visual images with kinetics (angular velocity, linear velocity, and gravity)
    OutputVisualKinematics: true
    # whether output the visual colorized point cloud map
    OutputColorizedMap: true
    # whether output aligned inertial measurements (aligned to the reference IMU)
    OutputAlignedInertialMes: true
    # supported data output format:
    # 0. JSON
    # 1. XML
    # 2. YAML
    # 3. BINARY (not recommended)
    # do not dwell on it, we have provide an additional ros program to perform data format tranform
    OutputDataFormat: "YAML"
    # number of thread to use for solving, negative value means use all valid thread to perform solving
    ThreadsToUse: -1
    # scale of splines in viewer, you can also use 'a' and 'd' keys to 
    # zoom out and in splines in run time
    SplineScaleInViewer: 3.0
    # scale of coordinates in viewer, you can also use 's' and 'w' keys 
    # to zoom out and in coordinates in run time
    CoordSScaleInViewer: 0.3
```
