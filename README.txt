OVERVIEW
--------
I created this application using Drupal 7.43 version. Basically I created
a custom module for this application. I just used drupal core and some
contributed module like- services, date. Everything is managing from custom module.

INSTALLATION
------------
First you have to unzip appointment.zip file. Then you have to create a database
called appointment ( you can use any name but you have to change the settings.php file from sites/defaults ).
Then you have to import db from db/appointment.sql.zip. I keep the file under db folder.

My settings.php file's configuration bellow-
$databases = array (
  'default' =>
  array (
    'default' =>
    array (
      'database' => 'appointment',
      'username' => 'root',
      'password' => 'root',
      'host' => 'localhost',
      'port' => '',
      'driver' => 'mysql',
      'prefix' => '',
    ),
  ),
);

You may have to change the information for username and password.

I also added my custom module for review under custom-module folder called appointment.


Note: I also added "appointment-dashboard" path as a home page in drupal.


LOGIN INFORMATION
-----------------
After installation you can login using the following information
User: admin
pass: 123


CUSTOM MODULE OVERVIEW
----------------------
I have created a module called "appointment" and also created a sub-module called "appointment_services"
under the main module. Main "appointment" module handles all front-end and backend section for this
application. We can add/edit/delete appointment. Validation are working properly for email and date.
Also I added a "Remind" link for Remainder for the appointee. I have a function , the function can send email.
But currently the function is as comment- for local install I can't use that. You can uncomment the function.

Note: Module is already installed with this project. But If you want to install only the module in any drupal project
The module have a install file for installation a table for this application.

/**
 * @param $id
 */
function remindAppointment($id)
{
    $result = db_select('appointment', 'a')
        ->fields('a')
        ->condition('id', $id)
        ->execute()
        ->fetchAssoc();

    if ($result) {

        //sendMailToAppointee($result);

        drupal_set_message('Remind email has been sent.', 'status');
        drupal_goto('appointment-dashboard');

    } else {

        drupal_set_message('The appointee not found.', 'status');
        drupal_goto('appointment-dashboard');
    }

}



REST SERVICES
=============
The "appointment_services" module handles all REST services for this application. I used drupal services module.
Then I just use the hook_services_resource() for my custom REST service. You can see my module.


Retrive
-------
To retrive from current date to next 30 days.
URL: {base_url}/api/v1/appointment

To retrive from input date to next 30 days.
URL: {base_url}/api/v1/appointment?date=2016-03-08
Method: GET
Header: Content-Type: application/json

Create
------
URL: {base_url}/api/v1/appointment
Method: POST
Header: Content-Type: application/json


Requested Json Format:
{
    "name": "Imran Sarder",
    "email": "imrancluster@gmail.com"
    "date": "2016-03-10"
}

Create
------
URL: {base_url}/api/v1/appointment/[appointment_id]
Method: PUT
Header: Content-Type: application/json


Requested Json Format:
{
    "name": "Imran Sarder",
    "email": "imrancluster@gmail.com"
    "date": "2016-03-10"
}

Update
------
URL: {base_url}/api/v1/appointment/[appointment_id]
Method: DELETE
Header: Content-Type: application/json
