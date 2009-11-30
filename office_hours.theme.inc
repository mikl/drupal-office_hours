<?php
//$Id$

/**
 * @file office_hours.theme.inc
 * Office hours theming functions.
 */

/**
 * Preprocess hook for default field formatter.
 */
function office_hours_preprocess_office_hours_formatter_default(&$vars) {
  $element = $vars['element'];
  $vars['node'] = $node = $element['#node'];

  $name = $element['#field_name'] . '_processed';
  $field_data = $node->$name;
  $vars['week'] = $field_data['week'];
  $vars['week_data'] = $field_data['week_data'];

  // Calculate week view, and prepare the variables we need to render it.
  $days = _office_hours_week_days($vars['week_data']['year_and_number']);
  $vars['start_day'] = format_date($days['start'], 'custom', 'd/m');
  $vars['end_day'] = format_date($days['end'], 'custom', 'd/m');
  $vars['day_abbr'] = date_week_days_ordered(_office_hours_day_names());
  $vars['day_names'] = date_week_days_ordered(date_week_days(TRUE));
  $vars['week_num_text'] = t('Week @week_num', array('@week_num' => $vars['week_data']['number']));

  drupal_add_js(array('officeHours' => array($vars['node']->nid => array(
    'year' => $vars['week_data']['year'],
    'week' => $vars['week_data']['number'],
    'nid' => $vars['node']->nid,
    'callback' => url('office_hours/get_week'),
    'field_name' => $element['#field_name'],
  ))), 'setting');
  drupal_add_js(drupal_get_path('module', 'office_hours') . '/office_hours.js');
  drupal_add_js(drupal_get_path('module', 'office_hours') . '/office_hours.display.js');

  return $vars;
}

/**
 * Theme function for the office hours week form element.
 */
function theme_office_hours_week($element) {
  drupal_add_js(drupal_get_path('module', 'office_hours') . '/office_hours.js');
  drupal_add_js(drupal_get_path('module', 'office_hours') . '/office_hours.form.js');
  drupal_add_css(drupal_get_path('module', 'office_hours') . '/office_hours.form.css');
  $header = array(t('Day'), t('Opening time'), t('Closing time'));
  $rows = array();

  $children = '';

  foreach ($element['#day_abbr'] as $key => $day) {
    // We need to re-render form elements, unset #printed attribute.
    unset($element[$day]['open']['#printed']);
    unset($element[$day]['close']['#printed']);

    $rows[$day] = array(
      $element['#day_names'][$key],
      drupal_render($element[$day]['open']),
      drupal_render($element[$day]['close']),
    );
  }

  unset($element['scope']['#printed'], $element['week_start']['#printed'], $element['week_end']['#printed']);

  $children .= '<div class="office-hour-rule-select clear-block">';
  $children .= drupal_render($element['scope']);
  $children .= drupal_render($element['week_start']);
  $children .= drupal_render($element['week_end']);
  $children .= '</div>';

  $children .= theme('table', $header, $rows, array('class' => 'office-hours-week'));

  return theme('form_element', $element, $children);
}

/**
 * Theme office hours for a specific date.
 *
 * @param string $name
 *    The name of the day.
 * @param string $open
 *    Open time.
 * @param string $close
 *    Close time.
 * @param integer $day_number
 *    Day number.
 * @return string
 *    HTML-formatted closing hours for that day.
 */
function theme_office_hours_format_day($name, $values, $day_number) {
  $oddity = ($day_number % 2) ? 'odd' : 'even';
  $output = '<div class="' . $oddity . '">';
  $output .= '<span class="day">' . $name . '</span>';
  if (is_array($values) && !empty($values)) {
    foreach ($values as $val) {
      $output .= ' <span class="hours start">' . office_hours_format_time($val['open']) . '</span>';
      $output .= ' – <span class="hours end">' . office_hours_format_time($val['close']) . '</span>';
    }
  }
  else {
    $output .= ' <span class="closed">' . t('closed') . '</span>';
  }
  return $output . '</div>';
}

