<?php
//$Id$

/**
 * @file office_hours.theme.inc
 * Office hours theming functions.
 */


/**
 * Theme function for 'default' text field formatter.
 */
function theme_office_hours_formatter_default($element) {   
  $output = '';
  $values = $element[0]['#item'];
  $day_abbr = date_week_days_ordered(_office_hours_day_names());
  $day_names = date_week_days_ordered(date_week_days(TRUE));

  foreach ($day_abbr as $num => $day) {
    if (!empty($values[$day . '_start']) && !empty($values[$day . '_end'])) {
      $output .= theme('office_hours_format_day', $day_names[$num], $values[$day . '_start'], $values[$day . '_end']);
    }
  }
  return $output;     
}

/**
 * Theme function for the office hours week form element.
 */
function theme_office_hours_week($element) {
  return $element['#children'];
}

/**
 * Theme function for the office hours day form element.
 */
function theme_office_hours_day($element) {
  return $element['#children'];
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
 * @return string
 *    HTML-formatted closing hours for that day.
 */
function theme_office_hours_format_day($name, $start, $end) {
  $output = '<div class="oh-display">' . $name;
  $output .= ' <span class="hours start">' . _office_hours_format_time($start) . '</span>';
  $output .= ' – <span class="hours end">' . _office_hours_format_time($end) . '</span></div>';
  return $output;
}

/**
 * copied from content module's theme_content_multiple values- we're taking out the draggable feature.
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

