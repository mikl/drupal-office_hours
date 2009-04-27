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
  $vars['week'] = _office_hours_resolve_week($vars['element']);
  $vars['week_data'] = array_shift($vars['week']);
  $days = _office_hours_week_days($vars['week_data']['year_and_number']);
  $vars['start_day'] = format_date($days['start'], 'custom', 'd/m');
  $vars['end_day'] = format_date($days['end'], 'custom', 'd/m');
  $vars['day_abbr'] = date_week_days_ordered(_office_hours_day_names());
  $vars['day_names'] = date_week_days_ordered(date_week_days(TRUE));
  $vars['week_num_text'] = t('Week @week_num', array('@week_num' => $vars['week_data']['number']));
  $vars['node'] = $vars['element']['#node'];

  drupal_add_js(array('officeHours' => array($vars['node']->nid => array(
    'year' => $vars['week_data']['year'],
    'week' => $vars['week_data']['number'],
    'nid' => $vars['node']->nid,
    'callback' => url('office_hours/js/change_week'),
    'field_name' => $vars['element']['#field_name'],
  ))), 'setting');
  drupal_add_js(drupal_get_path('module', 'office_hours') . '/office_hours.js');

  return $vars;
}

/**
 * Theme function for the office hours week form element.
 */
function theme_office_hours_week($element) {
  return theme('fieldset', $element);
}

/**
 * Theme function for the office hours day form element.
 *
 * Most of this is copied from theme_form_element.
 */
function theme_office_hours_day($element) {
  $output .= '<div class="' . $element['#attributes']['class'] . '"';
  if (!empty($element['#id'])) {
    $output .= ' id="'. $element['#id'] .'"';
  }
  $output .= '>';

  $required = !empty($element['#required']) ? '<span class="form-required" title="'. $t('This field is required.') .'">*</span>' : '';
  if (!empty($element['#title'])) {
    $output .= ' <h6>'. t('!title: !required', array('!title' => filter_xss_admin($element['#title']), '!required' => $required)) .'</h6>';
  }

  $output .= $element['#children'] . '</div>';
  return $output;
}

/**
 * Theme function for the office hours selector element.
 */
function theme_office_hours_select($element) {
  return theme('form_element', $element, $element['#children']);
}

/**
 * Theme office hours for a specific date.
 *
 * @param string $name
 *    The name of the day.
 * @param string $start
 *    Start time.
 * @param string $end
 *    End time.
 * @param integer $day_number
 *    Day number.
 * @return string
 *    HTML-formatted closing hours for that day.
 */
function theme_office_hours_format_day($name, $values, $day_number) {
  $oddity = ($day_number % 2) ? 'odd' : 'even';
  $output = '<div class="' . $oddity . '">' . $name;
  if (is_array($values) && !empty($values)) {
    foreach ($values as $val) {
      $output .= ' <span class="hours start">' . _office_hours_format_time($val['start']) . '</span>';
      $output .= ' – <span class="hours end">' . _office_hours_format_time($val['end']) . '</span>';
    }
  }
  else {
    $output .= ' <span class="closed">' . t('closed') . '</span>';
  }
  return $output . '</div>';
}

/**
 * Copied from content module's theme_content_multiple values- we're taking out the draggable feature.
 * Theme an individual form element.
 *
 */
function theme_office_hours_multiple_values($element) {
  $field_name = $element['#field_name'];
  $field = content_fields($field_name);
  $output = '';

  if ($field['multiple'] >= 1) {
    $table_id = $element['#field_name'] .'_values';
    $order_class = $element['#field_name'] .'-delta-order';
    $required = !empty($element['#required']) ? '<span class="form-required" title="'. t('This field is required.') .'">*</span>' : '';

    $header = array(
      array(
        'data' => t('!title: !required', array('!title' => $element['#title'], '!required' => $required)),
        'colspan' => 2
      ),
     // t('Order'),
    );
    $rows = array();

    // Sort items according to '_weight' (needed when the form comes back after
    // preview or failed validation)
    $items = array();
    foreach (element_children($element) as $key) {
      if ($key !== $element['#field_name'] .'_add_more') {
        $items[] = &$element[$key];
      }
    }
    usort($items, '_content_sort_items_value_helper');
    $items = _office_hours_arrange_day($items);
    // Add the items as table rows.
    foreach ($items as $key => $item) {
      $item['_weight']['#attributes']['class'] = $order_class;
      $delta_element = drupal_render($item['_weight']);
      $cells = array(
        array('data' => '', 'class' => 'content-multiple-drag'),
        drupal_render($item),
        //array('data' => $delta_element, 'class' => 'delta-order'),
      );
      
      $rows[] = array(
        'data' => $cells,
        //'class' => 'draggable',
      );
    }

    $output .= theme('table', $header, $rows, array('id' => $table_id, 'class' => 'content-multiple-table'));
    $output .= $element['#description'] ? '<div class="description">'. $element['#description'] .'</div>' : '';
    $output .= drupal_render($element[$element['#field_name'] .'_add_more']);

    //drupal_add_tabledrag($table_id, 'order', 'sibling', $order_class);
  }
  else {
    foreach (element_children($element) as $key) {
      $output .= drupal_render($element[$key]);
    }
  }

  return $output;
}

