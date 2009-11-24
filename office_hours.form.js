// $Id$

/**
 * @file
 * Drupal behavior for the office hours editing form.
 */

Drupal.behaviors.officeHoursForm = function () {
  $("table.office-hours-week input.form-text.time").change(function () {
    Drupal.officeHours.sanitiseTimeInput(this);
  });
};

