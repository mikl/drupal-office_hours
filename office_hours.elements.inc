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
  $elements = array(
    'office_hours_week' => array(
      '#input' => TRUE,
      '#tree' => TRUE,
      '#process' => array('office_hours_week_process'),
      '#element_validate' => array('office_hours_week_validate'),
    ),
  );

  // TODO: What is this?
  if (function_exists('office_hours_week_hs_process')) {
    $elements['office_hours_week']['#process'][] = 'office_hours_week_hs_process';
  }
  return $elements;
}

/**
 * Process the week element.
 *
 * Build the form element. When creating a form using FAPI #process,
 * note that $element['#value'] is already set.
 * The $fields array is in $form['#field_info'][$element['#field_name']].
 */
function office_hours_week_process($element, $edit, $form_state, $form) { 
  $values = $element['#value'];
  $element['#day_abbr'] = date_week_days_ordered(_office_hours_day_names());
  $element['#day_names'] = date_week_days_ordered(date_week_days(TRUE));

  $element['scope'] = array(
    '#type' => 'select',
    '#title' => t('Scope'),
    '#options' => array(
      'all' => t('Every week'),
      'odd' => t('Odd numbered weeks'),
      'even' => t('Even numbered weeks'),
      'week' => t('Specific week'),
      'week_range' => t('A range of weeks'),
    ),
    '#default_value' => $values['scope'],
  );

  $element['week_start'] = array(
    '#type' => 'textfield',
    '#title' => t('Start week'),
    '#default_value' => $values['week_start'],
    '#attributes' => array('class' => 'week-start'),
  );

  $element['week_end'] = array(
    '#type' => 'textfield',
    '#title' => t('End week'),
    '#default_value' => $values['week_end'],
    '#attributes' => array('class' => 'week-end'),
  );

  foreach ($element['#day_abbr'] as $key => $day) {
    $element[$day] = array(
      '#tree' => TRUE,
      '#prefix' => '<div class="oh-day oh-day-' . $day . ' clear-block">',
      '#suffix' => '</div>',
      '#day_name' => $element['#day_names'][$key],
    );

    $element[$day]['open'] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#maxlength' => 8,
      '#default_value' => office_hours_format_time($values[$day . '_open']),
      '#attributes' => array('class' => 'open time'),
    );

    $element[$day]['close'] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#maxlength' => 8,
      '#default_value' => office_hours_format_time($values[$day . '_close']),
      '#attributes' => array('class' => 'close time'),
    );
  }
  return $element;
}

/**
 * Validate the week element.
 */
function office_hours_week_validate($element, &$form_state) {
  $values = $element['#value'];

  foreach ($element['#day_abbr'] as $day) {
    if (!empty($values[$day]['open']) && !empty($values[$day]['close'])) {
      $open = office_hours_parse_time_input($values[$day]['open']);
      $close = office_hours_parse_time_input($values[$day]['close']);

      if (!empty($open) && !empty($close)) {
        $values[$day]['open'] = $open;
        $values[$day]['close'] = $close;
      }
      else {
        form_error($element[$day]['open'], t('Please enter open and close times in HH:MM format.'));
      }
    }
    else {
      unset($values[$day]);
    }
  }

  // For week_ranges, we need both start and end.
  if ($values['scope'] == 'week_range') {
    $values['week_start'] = office_hours_sanitise_week_input($values['week_start']);
    $values['week_end'] = office_hours_sanitise_week_input($values['week_end']);

    if (empty($values['week_start'])) {
      form_error($element['week_start'], t('You need to input a start week when selecting a week range.'));
    }

    if (empty($values['week_end'])) {
      form_error($element['week_end'], t('You need to input a end week when selecting a week range.'));
    }

    if ($values['week_end'] <= $values['week_start']) {
      form_error($element['week_end'], t('You need to input a end week later than the start week.'));
    }
  }
  // For a single week, we just need the start value.
  elseif ($values['scope'] == 'week') {
    $values['week_start'] = office_hours_sanitise_week_input($values['week_start']);
    unset($values['week_end']);

    if (empty($values['week_start'])) {
      form_error($element['week_start'], t('You need to input a start week when defining a week range.'));
    }
  }
  // For other scopes, remove the start and end values.
  else {
    unset($values['week_start'], $values['week_end']);
  }

  // Overwrite the values with our sanitised ones.
  form_set_value($element, $values, $form_state);
}

