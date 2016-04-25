<?php
/**
 * @file
 * appointment.inc
 */

/**
 * @param $form
 * @param $form_state
 * @param null $id
 * @return array
 */
function appointment_form($form, &$form_state, $id = NULL)
{
    $form = array();

    $name = $email = $date = '';
    $submit = 'Add Appointment';

    if ($id) {

        $fid_edit= db_select('appointment', 'a')
            ->fields('a')
            ->condition('id', $id)
            ->execute();

        while ($result = $fid_edit->fetchAssoc()) {
            $name           = $result['name'];
            $email          = $result['email'];
            $date           = date('Y-m-d', $result['date']);
        }

        $form['id'] = array('#type' => 'hidden', '#value' => $id);
        $submit = 'Update Appointment';
    }

    $format = 'Y-m-d';

    $form['date'] = array(
        '#type'                 => 'date_popup',
        '#default_value'        => date('c'),
        '#date_format'          => $format,
        '#date_timezone'        => 'Asia/Dhaka',
        '#date_increment'       => 15,
        '#date_year_range'      => '0:+6',
        '#default_value'        => $date,
        '#required'             => TRUE,
    );

    $form['name'] = array(
        '#title'            => t('Name'),
        '#type'             => 'textfield',
        '#default_value'    => $name,
        '#description'      => t('Name of appointee'),
        '#required'         => TRUE,

    );

    $form['email'] = array(
        '#title'            => t('Email'),
        '#type'             => 'textfield',
        '#default_value'    => $email,
        '#required'         => TRUE,
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t($submit)
    );

    return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function appointment_form_validate($form, &$form_state)
{

    if (!filter_var($form_state['values']['email'], FILTER_VALIDATE_EMAIL)) {
        form_set_error('email', t('Invalid email address.'));
    }

    if (isValidDateForAppointmet(trim($form_state['values']['date']))) {
        form_set_error('date', t('This date is already booked.'));
    }
}

/**
 * @param $form
 * @param $form_state
 * @throws Exception
 */
function appointment_form_submit($form, &$form_state)
{
    global $user;

    $action = (isset($form_state['values']['id']))?'edit':'add';

    switch ($action) {

        case 'add':
            $data = array(
                'name'          => $form_state['values']['name'],
                'email'         => $form_state['values']['email'],
                'date'          => strtotime($form_state['values']['date']),
                'user'          => $user->uid,
                'created'       => REQUEST_TIME,
                'updated'       => REQUEST_TIME,
            );

            addAppointment($data);

            drupal_set_message(t('The appointment has been created.'));

            break;

        case 'edit':

            $id = $form_state['values']['id'];

            $result = db_select('appointment', 'a')
                ->fields('a')
                ->condition('id', $id)
                ->execute()
                ->fetchAssoc();

            if ($result) {

                $data = array(
                    'name'          => $form_state['values']['name'],
                    'email'         => $form_state['values']['email'],
                    'date'          => strtotime($form_state['values']['date']),
                    'updated'       => REQUEST_TIME,
                );

                updateAppointment($data, $id);

                drupal_set_message(check_plain(t('Updated successfully')) . ' ' . $id);
            }

            break;
    }

    drupal_goto('appointment-dashboard');
}

/**
 * @return string
 * @throws Exception
 */
function appointmentDashboard()
{
    $limit = 10;

    $startDate  = strtotime(date('Y-m-d'));
    $endDate    = strtotime(date('Y-m-d', strtotime("+30 days")));

    $appointments = db_select('appointment', 'a')
        ->fields('a')
        ->condition('date', array($startDate, $endDate), 'BETWEEN')
        ->extend('PagerDefault')->limit($limit)
        ->execute();

    $items = array(
        'appointments'  => $appointments,
        'start'         => $startDate,
        'end'           => $endDate,
    );

    return theme( 'appointment_dashboard', array('items' => $items) );
}

/**
 * @param $id
 */
function deleteAppointment($id)
{
    $deleted = db_delete('appointment')
        ->condition('id', $id)
        ->execute();

    drupal_set_message('The appointment has been deleted.', 'status');

    drupal_goto('appointment-dashboard');
}

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


/**
 * Send email to appointee
 */
function sendMailToAppointee($appointee = []) {
    global $user, $base_url;

    $output = "<p>Dear " . $appointee['name'] . ",</p>";
    $output .= "<p>You have a appointment at Codemate at " . $appointee['date'] . ".</p>";
    $output .= "<p>If you need further any query feel free inform us.</p>";
    $output .= "<p>Regards<br>Codemate Ltd.</p>";

    $body   = $output;
    $to     = $appointee['email'];
    $from   = variable_get('site_mail', 'imrancluster@gmail.com');

    $header_bcc = '';
    $header_cc  = '';
    $subject    = 'Appointment at Codemate';

    $message = drupal_mail(
        'appointment',
        'sendMailToAppointee',
        $to,
        user_preferred_language($user),
        array(
            'body'      => $body,
            'subject'   => $subject,
            'headers'   => array(
                'Cc'                        => $header_cc,
                'Bcc'                       => $header_bcc,
                'Content-Type'              => 'text/html; charset=UTF-8; format=flowed',
            ),
            $from,
            TRUE
        )
    );

}