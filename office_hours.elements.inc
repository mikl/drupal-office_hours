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

  foreach ($day_abbr as $num => $day) {
    $element[$day] = array(
      '#type' => 'date_day',
    );
  }

  /*$iteration = $element['#weight'];
  $field = $form['#field_info'][$element['#field_name']];
  if ($field['multiple'] > 7) {
    $day = ($iteration == 0) ? 0 :(($iteration & 1) ? ($iteration-1)/2 : $iteration/2);
	}
  else {
  	$day = $iteration;
 }
  $field_day = $element['#columns'][0];
  $field_strhrs = $element['#columns'][1];
  $field_endhrs = $element['#columns'][2];  
  $delta = $element['#delta'];
  #$optionshr= _create_hours_arr($field, TRUE);
  $optionday= array(0 => t('Sunday'), 1 => t('Monday'), 2 => t('Tuesday'),3 => t('Wednesday'), 4 => t('Thursday'), 5 => t('Friday'), 6 => t('Saturday'), 7=> t('None'));
 $element['#prefix'] = '<div class="office-hours-block" style="display:inline">';
 
 if ( $field['multiple'] == '7' || ($field['multiple'] == '14' && !($iteration & 1 )) ) {
    $element['#prefix'] .= t($optionday[$day]);
  }
  else {
     $element['#prefix'] .= t('Additional hours:');
  }
  
  $element['#suffix'] = '</div>';

  $element[$field_day] = array(
    '#type' => 'value',
    '#value' => $day,
 );
 $element[$field_strhrs] = array(
    '#type' => 'select',
    '#title' => t('Open from'),
    '#description' => t($field['widget']['description']),
    '#options' => $optionshr,
    '#required' => $field['required'],
    '#default_value' => ($element['#value']['day'] == $day && isset($element['#value'][$field_strhrs])) ? $element['#value'][$field_strhrs] : '',
 );
 $element[$field_endhrs] = array(
    '#type' => 'select',
    '#title' => t('Until').$suffix,
    '#description' => t($field['widget']['description']),
    '#options' =>  $optionshr,
    '#required' => $field['required'],
    '#default_value' => ($element['#value']['day'] == $day && isset($element['#value'][$field_endhrs])) ? $element['#value'][$field_endhrs] : '',
 );
 $form_state['#field_info'][$field['field_name']] = $field;*/
  return $element;
}

/**
 * Process the office hour day element.
 */
function office_hours_day_process($element) {
  $hours = date_hours();
  $minutes = date_minutes('i', FALSE, 30);

  $element['hours_start'] = array(
    '#type' => 'select',
    '#title' => t('Open from'),
    '#options' => $hours,
  );

  $element['minutes_start'] = array(
    '#type' => 'select',
    '#options' => $minutes,
  );

  $element['hours_end'] = array(
    '#type' => 'select',
    '#title' => t('Open to'),
    '#options' => $hours,
  );

  $element['minutes_end'] = array(
    '#type' => 'select',
    '#options' => $minutes,
  );
  return $element;
}

