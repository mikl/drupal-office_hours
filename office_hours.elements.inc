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
  );
}

/*
 * Process an individual element.
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
 * Process the office hour day element.
 */
function office_hours_day_process($element) {
  $hours = date_hours();
  $minutes = date_minutes('i', FALSE, 30);

  $element['#prefix'] = '<div class="oh-day ' . $element['#day_name'] . '">';
  $element['#suffix'] = '</div>';

  $element['title'] = array(
    '#value' => '<h4 class="title">' . $element['#title'] . '</h4>',
  );

  $element['hours_start'] = array(
    '#type' => 'select',
    '#options' => $hours,
    '#prefix' => '<div class="from clear-block"><label>' . t('From') . '</label>',
  );

  $element['minutes_start'] = array(
    '#type' => 'select',
    '#options' => $minutes,
    '#suffix' => '</div>',
  );

  $element['hours_end'] = array(
    '#type' => 'select',
    '#options' => $hours,
    '#prefix' => '<div class="to clear-block"><label>' . t('To') . '</label>',
  );

  $element['minutes_end'] = array(
    '#type' => 'select',
    '#options' => $minutes,
    '#suffix' => '</div>',
  );

  # Default values need a bit of special treatment here, since the value 
  # that's coming from the database is a zerofilled string that does not 
  # match the options, since they are cast to integer by Drupal.
  if (!empty($element['#default_start'])) {
    $start_val = explode(':', $element['#default_start']);
    if (isset($start_val[0]) && !empty($start_val[0])) {
      $element['hours_start']['#default_value'] = intval($start_val[0]);
    }
    if (isset($start_val[1]) && !empty($start_val[1])) {
      $element['minutes_start']['#default_value'] = intval($start_val[1]);
    }
  }
  if (!empty($element['#default_end'])) {
    $end_val = explode(':', $element['#default_end']);
    if (isset($end_val[0]) && !empty($end_val[0])) {
      $element['hours_end']['#default_value'] = intval($end_val[0]);
    }
    if (isset($end_val[1]) && !empty($end_val[1])) {
      $element['minutes_end']['#default_value'] = intval($end_val[1]);
    }
  }
  return $element;
}

