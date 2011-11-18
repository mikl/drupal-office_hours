/**
 * @file
 * Drupal behavior for the office hours editing form.
 */

(function ($) {
"use strict";

/**
 * Callback to sanitise a text input field for time values.
 */
Drupal.officeHours.sanitiseTimeInput = function (elem) {
  var $this = $(elem),
      newVal = '',
      // Try matching for full time.
      match = $this.val().match(Drupal.officeHours.timeMatcher);

  if (match) {
    // match[1] is hours, match[2] is minutes.
    // Prefix zero to single-digit hours.
    if (match[1] < 10) {
      newVal += '0' + parseInt(match[1], 10);
    }
    else if (match[1] < 24) {
      newVal += match[1];
    }
    // If set to more than 23, go back down.
    else {
      newVal += 23;
    }

    if (match[2] < 60) {
      newVal += ':' + match[2];
    }
    else {
      newVal += ':59';
    }
  }
  else {
    // Try matching just an hours value.
    match = $this.val().match(Drupal.officeHours.twoDigitMatcher);
    if (match) {
      // Prefix zero to single-digit hours.
      if (match[1] < 10) {
        newVal += '0' + parseInt(match[1], 10);
      }
      else if (match[1] < 24) {
        newVal += match[1];
      }
      // If set to more than 23, go back down.
      else {
        newVal += 23;
      }

      newVal += ':00';
    }
  }

  $this.val(newVal);
};

/**
 * Callback to sanitise a text input field for week values.
 */
Drupal.officeHours.sanitiseWeekInput = function (elem) {
  var $this = $(elem),
      newVal = '',
      match = $this.val().match(Drupal.officeHours.yearWeekMatcher),
      // Find max week no of year
      weekObj = new Date(match[1], 11, 31),
      weekno = weekObj.getWeek(),
      maxWeek = weekno + 1,
      year;

  if (match) {
    newVal += parseInt(match[1], 10);
    newVal += 'W';

    // Prefix zero to single-digit weeks.
    if (match[2] < 10) {
      newVal += '0' + parseInt(match[2], 10);
    }
    // We dont always have 53 weeks per year
    else if (match[2] < maxWeek) {
      newVal += match[2];
    }
    else {
      newVal += '01';
    }
  }
  else {
    // If we couldn't match a full value, try finding a week number.
    match = $this.val().match(Drupal.officeHours.twoDigitMatcher);
    // We dont always have 53 weeks per year
    if (match && match[1] < maxWeek) {
      year = new Date().getFullYear();
      newVal += year + 'W' + match[1];
    }
  }

  $this.val(newVal);
};

Drupal.behaviors.officeHoursForm = function () {
  $("table.office-hours-week input.form-text.time").change(function () {
    Drupal.officeHours.sanitiseTimeInput(this);
  });

  $('.office-hour-rule-select input.form-text').change(function () {
    Drupal.officeHours.sanitiseWeekInput(this);
  });

  $('.office-hour-rule-select select').change(function () {
    var $this = $(this),
        $wrapper = $this.parents('.office-hour-rule-select');

    // For the week range, show both text fields.
    if ($this.val() === 'week_range') {
      $wrapper.find('input.form-text').parent().show('fast');
    }
    // For the week, show only the start field.
    else if ($this.val() === 'week') {
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

  var $select = $(".tabledrag-processed .office-hour-rule-select");

  // In case of multiple values, add a delete button to each rule.
  $select.prepend('<a class="delete" href="#" title="' + Drupal.t('Delete') + '">×</a>');
  $select.find('a.delete').click(function () {
    // On click, fade the draggable container out before removing.
    $(this).parents('tr.draggable').fadeOut('normal', function () {
      // Empty all the time inputs, since that will cause the field to
      // be considered empty by CCK, and be deleted.
      $(this).find('input.time').val('');
    });

    // If not done already, mark the tableDrag as changed.
    var td = Drupal.tableDrag[$select.parents('table.tabledrag-processed').attr('id')];
    if (!td.changed) {
      $(Drupal.theme('tableDragChangedWarning')).insertAfter(td.table).hide().fadeIn('slow');
      td.changed = true;
    }
    return false;
  });
};

}(jQuery));

