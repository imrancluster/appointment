<?php
/**
 * @file
 * appointment.resource.inc
 */

function appointmentRetrieve()
{

    if ($_GET['date']) {

        $startDate  = strtotime($_GET['date']);
        $endDate    = strtotime(date('Y-m-d', strtotime($_GET['date'] .' +1 day')));

        $results = db_select('appointment', 'a')
            ->fields('a')
            ->condition('date', array($startDate, $endDate), 'BETWEEN')
            ->execute();

        $data = [];

        foreach ($results as $result) {

            $data[] = array(
                'date'  => date('Y-m-d', $result->date),
                'name'  => $result->name,
                'email' => $result->email,
            );
        }

        return $data;


    } else {

        $startDate  = strtotime(date('Y-m-d'));
        $endDate    = strtotime(date('Y-m-d', strtotime("+30 days")));

        $results = db_select('appointment', 'a')
            ->fields('a')
            ->condition('date', array($startDate, $endDate), 'BETWEEN')
            ->execute();

        $data = [];

        foreach ($results as $result) {

            $data[] = array(
                'date'  => date('Y-m-d', $result->date),
                'name'  => $result->name,
                'email' => $result->email,
            );
        }

        return $data;

    }
}

function appointmentCreate($appointment)
{
    try {

        isValidEmail($appointment);

        if (isValidDateForAppointmet($appointment['email'])) {
            return 'This date is already booked.';
        }

        addAppointment($appointment);

        return "Success";

    } catch (Exception $e) {

        return $e;
    }
}

function appointmentUpdate($appointment_id, $appointment)
{
    try {

        isValidEmail($appointment);

        if (isValidDateForAppointmet($appointment['email'])) {
            return 'This date is already booked.';
        }

        updateAppointment($appointment, $appointment_id);

        return "Success";

    } catch (Exception $e) {

        return $e;
    }
}


function appointmentDelete($appointment_id)
{
    $deleted = db_delete('appointment')
        ->condition('id', $appointment_id)
        ->execute();
    if ($deleted) {
        return "Success";
    } else {
        return 404;
    }
}

function isValidEmail($appointment)
{
    if (!filter_var($appointment['email'], FILTER_VALIDATE_EMAIL)) {
        return "Invalid email address.";
    }
}