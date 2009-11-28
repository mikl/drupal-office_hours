// $Id$

/**
 * @file
 * Drupal behavior for the office hours editing form.
 */

/**
 * Callback to sanitise a text input field for time values.
 */
Drupal.officeHours.sanitiseTimeInput = function (elem) {
  var $this = $(elem);
  var newVal = '';

  // Try matching for full time.
  var match = $this.val().match(Drupal.officeHours.timeMatcher);
  if (match) {
    // match[1] is hours, match[2] is minutes.
    // Prefix zero to single-digit hours.
    if (match[1] < 10) {
      newVal += '0' + parseInt(match[1]);
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
    var match = $this.val().match(Drupal.officeHours.twoDigitMatcher);
    if (match) {
      // Prefix zero to single-digit hours.
      if (match[1] < 10) {
        newVal += '0' + parseInt(match[1]);
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
  var $this = $(elem);
  var newVal = '';

  var match = $this.val().match(Drupal.officeHours.yearWeekMatcher);
  if (match) {
    newVal += parseInt(match[1]);
    newVal += 'W';

    // Prefix zero to single-digit weeks.
    if (match[2] < 10) {
      newVal += '0' + parseInt(match[2]);
    }
    else if (match[2] < 54) {
      newVal += match[2];
    }
    else {
      newVal += '01';
    }
  }
  else {
    // If we couldn't match a full value, try finding a week number.
    var match = $this.val().match(Drupal.officeHours.twoDigitMatcher);
    if (match && match[1] < 54) {
      var year = new Date().getFullYear();
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

