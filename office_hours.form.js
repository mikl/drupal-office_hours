// $Id$

/**
 * @file
 * Drupal behavior for the office hours editing form.
 */

Drupal.behaviors.officeHoursForm = function () {
  $("table.office-hours-week input.form-text.time").change(function () {
    Drupal.officeHours.sanitiseTimeInput(this);
  });

  $('.office-hour-rule-select input.form-text').change(function () {
    Drupal.officeHours.sanitiseWeekInput(this);
  });

  $('.office-hour-rule-select select').change(function () {
    var $this = $(this);
    var $wrapper = $this.parents('.office-hour-rule-select');

    // For the week range, show both text fields.
    if ($this.val() == 'week_range') {
      $wrapper.find('input.form-text').parent().show('fast');
    }
    // For the week, show only the start field.
    else if ($this.val() == 'week') {
      $wrapper.find('input.week-start').parent().show('fast');
      $wrapper.find('input.week-end').parent().hide('fast');
    }
    // For all other options, hide both.
    else {
      $wrapper.find('input.form-text').parent().hide('fast');
    }
  // Trigger an initial change event to the the text field to the
  // correct state.
  }).change();
};

