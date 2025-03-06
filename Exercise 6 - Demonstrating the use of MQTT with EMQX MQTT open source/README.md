# Completing MQTT Exercise on Arduino
## Requisites:
- Docker is installed and working
- Arduino IDE is installed; and
- The following library is installed: `Arduinojson`
- The following files are installed: `docker-compose.yml`, `exercise-6.ino`, `exercise-6-json.ino`
- Both Laptop/PC and Arduino are connected to a mobile hotspot
## Exercise
- In the same folder that the files are intstalled, run the following command using PowerShell `docker compose up -d` (this will pull the images from docker hub and spin up the containers for emqx and mqttx service)
- Verify that the containers are running by visiting [http://localhost:18083/](http://localhost:18083/) and [http://localhost:80/](http://localhost:80/)
- Create a database in the [Authentication page](http://localhost:18083/#/authentication) with defaults selected
- Create a user with username `arduino` and password `pass`
- In `exercise-6.ino` and `exercise-6-json.ino`, replace "hotspot_name" and "hotspot pass" with your hotspot SSID and password, it should look like this:
  ```
  char ssid[] = "hotdogwifi"; // replace placeholder
  char pass[] = "12345678"; // replace placeholder
  ```
- With the same files opened, replace "192.168.X.X" with your private IP Address, you can do this by running `ipconfig | findstr IPv4` on PowerShell or `hostname -I | awk '{print $1}'` on BASH, it should look like this:
  ```
  const char broker[] = "192.168.123.123"; // replace placeholder
  ```
- Follow through with the [Tutorial](https://www.hackster.io/virgilvox/mqtt-with-the-arduino-uno-r4-wifi-and-emqx-2639f9#story) to complete the Exercise
