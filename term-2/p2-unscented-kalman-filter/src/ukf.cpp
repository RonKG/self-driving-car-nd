#include "ukf.h"
#include "tools.h"
#include "Eigen/Dense"
#include <iostream>

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::vector;

/**
 * Initializes Unscented Kalman filter
 */
UKF::UKF() {
  // if this is false, laser measurements will be ignored (except during init)
  use_laser_ = true;

  // if this is false, radar measurements will be ignored (except during init)
  use_radar_ = true;

  // initial state vector
  x_ = VectorXd(5);

  // initial covariance matrix
  P_ = MatrixXd(5, 5);

  // Process noise standard deviation longitudinal acceleration in m/s^2
  std_a_ = 30;

  // Process noise standard deviation yaw acceleration in rad/s^2
  std_yawdd_ = 30;

  // Laser measurement noise standard deviation position1 in m
  std_laspx_ = 0.15;

  // Laser measurement noise standard deviation position2 in m
  std_laspy_ = 0.15;

  // Radar measurement noise standard deviation radius in m
  std_radr_ = 0.3;

  // Radar measurement noise standard deviation angle in rad
  std_radphi_ = 0.03;

  // Radar measurement noise standard deviation radius change in m/s
  std_radrd_ = 0.3;

  /**
  TODO:

  Complete the initialization. See ukf.h for other member properties.

  Hint: one or more values initialized above might be wildly off...
  */

}

UKF::~UKF() {}

/**
 * @param {MeasurementPackage} meas_package The latest measurement data of
 * either radar or laser.
 */
void UKF::ProcessMeasurement(MeasurementPackage measurement_pack) {
  /**
  TODO:

  Complete this function! Make sure you switch between lidar and radar
  measurements.
  */
    
  /*****************************************************************************
   *  Initialization
   ****************************************************************************/

  if (!is_initialized_) {
    /**
    TODO:
      * Initialize the state x_ with the first measurement.
      * Create the covariance matrix.
      * Remember: you'll need to convert radar from polar to cartesian coordinates.
    */
    // first measurement
    cout << "UKF: " << endl;
    x_ << 1, 1, 1, 1, 1;
      
    // Create process covariance matrix
    // TODO: Unsure about dimensions of Q.
    Q_ = Eigen::MatrixXd(4,4);

    float px;
    float py;

    if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
      /**
      Convert radar from polar to cartesian coordinates and initialize state.
      */
        cout << "init radar" << endl;

        float rho = measurement_pack.raw_measurements_[0];
        float phi = measurement_pack.raw_measurements_[1];
        
        px = rho*cos(phi);
        py = rho*sin(phi);

    }
    else if (measurement_pack.sensor_type_ == MeasurementPackage::LASER) {
      /**
      Initialize state.
      */
        cout << "init laser" << endl;

        px = measurement_pack.raw_measurements_[0];
        py = measurement_pack.raw_measurements_[1];

    }

    // Handle small px, py
    if(fabs(px) < 0.0001){
        px = 0.1;
        cout << "init px too small" << endl;
    }

    if(fabs(py) < 0.0001){
        py = 0.1;
        cout << "init py too small" << endl;
    }


    x_ << px, py, 0, 0, 0;
    cout << "x_: " << x_ << endl;

    previous_timestamp_ = measurement_pack.timestamp_;
    // done initializing, no need to predict or update
    is_initialized_ = true;
    return;
  }

  /*****************************************************************************
   *  Update
   ****************************************************************************/

  /**
   TODO:
     * Use the sensor type to perform the update step.
     * Update the state and covariance matrices.
   */

  if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
    // Radar updates
      cout << "Radar update" << endl;
      
      hx_ = VectorXd(3);
      
      float px = x_[0];
      float py = x_[1];
      float vx = x_[2];
      float vy = x_[3];

      float rho;
      float phi;
      float rhodot;

      // Handling small measurements

      if(fabs(px) < 0.0001 or fabs(py) < 0.0001){
        
        if(fabs(px) < 0.0001){
          px = 0.0001;
          cout << "px too small" << endl;
        }

        if(fabs(py) < 0.0001){
          py = 0.0001;
          cout << "py too small" << endl;
        }
        
        rho = sqrt(px*px + py*py);
        phi = 0;
        rhodot = 0;
  
      } else {
        rho = sqrt(px*px + py*py);
        phi = atan2(py,px); //  arc tangent of y/x, in the interval [-pi,+pi] radians.
        rhodot = (px*vx + py*vy) /rho;
      }      

      // I don't even know what hx_ is for
      hx_ << rho, phi, rhodot;
      
      /*
      // set H_ to Hj when updating with a radar measurement
      // NO MORE JACOBIAAAAN
      // Hj_ = tools.CalculateJacobian(x_);
      
      // don't update measurement if we can't compute the Jacobian
      if (Hj_.isZero(0)){
        cout << "Hj is zero" << endl;
        return;
      }
      */
      
      // Todo: What happens to H?
      // H_ = Hj_;

      // R_ = R_radar_;

      UpdateRadar(measurement_pack);   
    
      
  } else {
    // Laser updates
      cout << "Laser update" << endl;
      
      UpdateLidar(measurement_pack);
  }
    
    
}

/**
 * Predicts sigma points, the state, and the state covariance matrix.
 * @param {double} delta_t the change in time (in seconds) between the last
 * measurement and this one.
 */
void UKF::Prediction(double delta_t) {
  /**
  TODO:

  Complete this function! Estimate the object's location. Modify the state
  vector, x_. Predict sigma points, the state, and the state covariance matrix.
  */
}

/**
 * Updates the state and the state covariance matrix using a laser measurement.
 * @param {MeasurementPackage} meas_package
 */
void UKF::UpdateLidar(MeasurementPackage meas_package) {
  /**
  TODO:

  Complete this function! Use lidar data to update the belief about the object's
  position. Modify the state vector, x_, and covariance, P_.

  You'll also need to calculate the lidar NIS.
  */
}

/**
 * Updates the state and the state covariance matrix using a radar measurement.
 * @param {MeasurementPackage} meas_package
 */
void UKF::UpdateRadar(MeasurementPackage meas_package) {
  /**
  TODO:

  Complete this function! Use radar data to update the belief about the object's
  position. Modify the state vector, x_, and covariance, P_.

  You'll also need to calculate the radar NIS.
  */
}