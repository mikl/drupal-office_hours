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
    '#description' => t('Select which weeks these hours apply to'),
    '#options' => array(
      'all' => t('Every week'),
      'odd' => t('Odd numbered weeks'),
      'even' => t('Even numbered weeks'),
      'week' => t('Specific week'),
      'week_range' => t('A range of weeks'),
    ),
    '#default_value' => $values['scope'],
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
      '#default_value' => $values[$day . '_open'],
      '#attributes' => array('class' => 'open time'),
    );

    $element[$day]['close'] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#maxlength' => 8,
      '#default_end' => $values[$day . '_close'],
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
      $open = office_hours_parse_input($values[$day]['open']);
      $close = office_hours_parse_input($values[$day]['close']);

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

  // Overwrite the values with our sanitised ones.
  form_set_value($element, $values, $form_state);
  dpm($form_state);
}

