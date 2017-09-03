#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>
#include "fstream"
#include <ctime>


#define cnt_per_iter 500
#define tweedle 0


// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

int interation = 1;
int iteration_no_update;
double throttle = 0.3;
double best_error = 999999;

int max_iter = -1;

std::string now( const char* format = "%c" )
{
  std::time_t t = std::time(0) ;
  char cstr[128] ;
  std::strftime( cstr, sizeof(cstr), format, std::localtime(&t) ) ;
  return cstr ;
}

std::ofstream logfile("logfile_" + now( "%Y_%m_%d_%H:%M:%S.txt"));
//std::ofstream pidlogfile("pidlogfile_" + now( "%Y_%m_%d_%H:%M:%S.txt"));


// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main(int argc, char *argv[])
{
  uWS::Hub h;
  double p = 0.13, i = 0.0001, d = 4;

  if (argc > 1)
  {
    p = atof(argv[1]);
    i = atof(argv[2]);
    d = atof(argv[3]);

    throttle = atof(argv[4]);
    max_iter = atof(argv[5]);


  }

  std::cout << "p=" << p << " i=" << i << "d=" << d << std::endl;


  PID pid;
  // TODO: Initialize the pid variable.
  pid.Init(p, i, d);



  h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value = pid.Steering(cte);

          interation++;
          if (steer_value > 1) steer_value = 1;
          if (steer_value < -1) steer_value = -1;


          //twiddle
          if (interation == 1 && tweedle)
          {
            std::cout << "Iteration " << 0  <<
                      " PID=" << pid.KPID << " dkPID=" << pid.dKPID << std::endl;
          }


          json logJson;
          logJson["steering_angle"] = steer_value;
          logJson["throttle"] = throttle;
          logJson["speed"] = speed;
          logJson["cte"] = cte;
          logJson["angle"] = angle;
          logJson["diffcte"] = pid.diff_cte;
          logJson["iteration"] = interation;
          logJson["total_error"] = pid.int_cte;

          logfile << logJson.dump() << std::endl;


          json msgJson;
          msgJson["steering_angle"] = steer_value;

          //for trottle
          double tr = throttle;
          //if (fabs(cte)>0.5) tr=0.1;

          //max_iteration
          if (max_iter > 0 && interation > max_iter) {
            std::cout << "max itreations!";
            exit (0);
          }

          msgJson["throttle"] = tr;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          //std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse * res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
