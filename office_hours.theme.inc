<?php
//$Id$

/**
 * @file office_hours.theme.inc
 * Office hours theming functions.
 */

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
  $output = array('<div class="' . $oddity . ' clear-block">');
  $output[] = '<span class="day">' . $name . '</span>';
  if (is_array($values) && !empty($values)) {
    foreach ($values as $val) {
      $output[] = '<span class="hours">';
      $output[] = ' <span class="start">' . office_hours_format_time($val['open']) . '</span>';
      $output[] = ' – <span class="end">' . office_hours_format_time($val['close']) . '</span>';
      $output[] = '</span>';
    }
  }
  else {
    $output[] = ' <span class="hours closed">' . t('closed') . '</span>';
  }
  $output[] = '</div>';
  return join("\n", $output);
}

