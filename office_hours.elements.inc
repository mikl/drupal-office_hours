<?php
//$Id$

/**
 * @file office_hours.elements.inc
 * Office hours form elements and their theming and validation.
 * This file is only included during the edit process to reduce memory usage.
 */


/**
 * Implementation of hook_elements().
 */
function _office_hours_elements() {
  return array(
    'office_hours_week' => array(
      '#input' => TRUE,
      '#tree' => TRUE,
      '#process' => array('office_hours_week_process'),
      '#element_validate' => array('office_hours_week_validate'),
    ),
    'office_hours_day' => array(
      '#input' => TRUE,
      '#tree' => TRUE,
      '#process' => array('office_hours_day_process'),
      '#element_validate' => array('office_hours_day_validate'),
    ),
    'office_hours_select' => array(
      '#input' => TRUE,
      '#tree' => TRUE,
      '#process' => array('office_hours_select_process'),
      '#element_validate' => array('office_hours_select_validate'),
    ),
  );
}

/*
 * Process the week element.
 *
 * Build the form element. When creating a form using FAPI #process,
 * note that $element['#value'] is already set.
 * The $fields array is in $form['#field_info'][$element['#field_name']].
 */
function office_hours_week_process($element, $edit, $form_state, $form) { 
  $values = $element['#value'];
  $day_abbr = date_week_days_ordered(_office_hours_day_names());
  $day_names = date_week_days_ordered(date_week_days(TRUE));

  $element['#prefix'] = '<div class="oh-week element clear-block">';
  $element['#suffix'] = '</div>';

  foreach ($day_abbr as $num => $day) {
    $element[$day] = array(
      '#type' => 'office_hours_day',
      '#title' => $day_names[$num],
      '#default_start' => $values[$day . '_start'],
      '#default_end' => $values[$day . '_end'],
      '#day_name' => $day,
      '#day_number' => 'day-' . $num,
    );
  }
  return $element;
}

/**
 * Validate the week element.
 */
function office_hours_week_validate($element, &$form_state) {
}

/**
 * Process the office hour day element.
 */
function office_hours_day_process($element) {
  $day = $element['#day_name'];

  _form_set_class($element, array('oh-day ' . $day));

  $element['start'] = array(
    '#type' => 'office_hours_select',
    '#title' => t('From'),
    '#default_value' => $element['#default_start'],
  );

  $element['end'] = array(
    '#type' => 'office_hours_select',
    '#title' => t('To'),
    '#default_value' => $element['#default_end'],
  );

  return $element;
}

/**
 * Validate the day element.
 */
function office_hours_day_validate($element, &$form_state) {
  $day = $element['#day_name'];
  $start = $element['#value']['start'];
  $end = $element['#value']['end'];
  $values = array();
  if (!empty($start['hours']) && !empty($end['hours'])) {
    if (intval($start['hours']) > intval($end['hours']) || (intval($start['hours']) == intval($end['hours']) && (intval($start['minutes']) >= intval($end['minutes'])))) {
      form_error($element, t('End hours cannot occur before start hours.'));
    }
    else {
      $values['start'] = sprintf('%02u:%02u', $start['hours'], $start['minutes']);
      $values['end'] = sprintf('%02u:%02u', $end['hours'], $end['minutes']);
    }
  }
  form_set_value($element, $values, $form_state);
}

/**
 * Process the hours selector element.
 */
function office_hours_select_process($element) {
  $hours = date_hours();
  $minutes = date_minutes('i', FALSE, 30);
  $day = $element['#day_name'];


  $element['hours'] = array(
    '#type' => 'select',
    '#options' => $hours,
  );

  $element['minutes'] = array(
    '#type' => 'select',
    '#options' => $minutes,
  );

  # Default values need a bit of special treatment here, since the value 
  # that's coming from the database is a zerofilled string that does not 
  # match the options, since they are cast to integer by Drupal.
  if (!empty($element['#default_value'])) {
    $end_val = explode(':', $element['#default_value']);
    if (isset($end_val[0]) && !empty($end_val[0])) {
      $element['hours']['#default_value'] = intval($end_val[0]);
    }
    if (isset($end_val[1]) && !empty($end_val[1])) {
      $element['minutes']['#default_value'] = intval($end_val[1]);
    }
  }
  return $element;
}

/**
 * Validate the hours selector element.
 */
function office_hours_select_validate($element, &$form_state) {
  $hours = intval($element['#value']['hours']);
  $minutes = intval($element['#value']['minutes']);


  if ($hours < 0 || $hours > 23) {
    form_error($element, t('Hours should be between 0 and 23.'));
  }
  if ($minutes < 0 || $minutes > 59) {
    form_error($element, t('Minutes should be between 0 and 59.'));
  }
}

