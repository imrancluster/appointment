<section class="appointment-dashboard">
    <h1><?php echo date('F d, Y', $items['start']) ?> to <?php echo date('F d, Y', $items['end']) ?></h1>
    <table class="table table-striped">
        <thead>
        <tr>
            <th>Date</th>
            <th>Appointment</th>
            <th>Action</th>
        </tr>
        </thead>
        <tbody>
        <?php

        $appointments = isset($items['appointments']) ? $items['appointments'] : [];

        foreach($appointments as $appointmet):
            ?>
            <tr>
                <td><?php echo date('Y-m-d', $appointmet->date); ?></td>
                <td><?php echo $appointmet->name . ", " . $appointmet->email; ?></td>
                <td>
                    <a href="/appointment-dashboard/<?php echo $appointmet->id; ?>/edit">Edit</a> |
                    <a class="remove-item" href="/appointment/<?php echo $appointmet->id; ?>/delete">Delete</a> |
                    <a href="/appointment/<?php echo $appointmet->id; ?>/remind">Remind</a>
                </td>
            </tr>
        <?php endforeach; ?>
        </tbody>
    </table>
    <?php print theme('pager', array('tags' => array())); ?>

</section>
<section class="add-appointment">

    <?php
        $form_view  = drupal_get_form('appointment_form');
        $output     = drupal_render($form_view);

        print $output;
    ?>


</section>