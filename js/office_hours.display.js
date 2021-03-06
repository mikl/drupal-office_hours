/**
 * @file
 * Javascript helpers for the office hours module.
 */

(function ($) {
"use strict";

/**
 * Office Hours Drupal JavaScript behaviour
 *
 * attaches the scrolling arrows to the office hours elements.
 */
Drupal.behaviors.officeHours = function () {
  $(".office-hours-week:not(.oh-processed)").each(function () {
    var $this, match;
    $this = $(this);
    match = $this.attr('class').match(Drupal.officeHours.nidMatcher);
    if (match && match[1] > 0) {
      $this.addClass('oh-processed')
        .find('.week-info')
          .prepend('<a class="prev" href="#">&larr;</a>')
          .append('<a class="next" href="#">&rarr;</a>')
        .end()
        .find('a.prev')
          .click(function () {return Drupal.officeHours.changeWeek(match[1], 'prev');})
        .end()
        .find('a.next')
          .click(function () {return Drupal.officeHours.changeWeek(match[1], 'next');})
        .end();
      }
  });
};

/**
 * Helper function to get week no from a date object
 */
Date.prototype.getWeek = function() {
  var onejan = new Date(this.getFullYear(), 0, 1);
  return Math.ceil((((this - onejan) / 86400000) + onejan.getDay() + 1) / 7);
};

/**
 * Helper function to change office hours view to a different week.
 *
 * Calls Drupal's JSON callback to get the new data.
 */
Drupal.officeHours.changeWeek = function (nid, direction) {
  var conf, week, lastDayOfYear, weekCount;
  conf = Drupal.settings.officeHours['node' + nid];

  // Find amount of weeks in selected year, because there is not always 53 weeks in a year
  lastDayOfYear = new Date(conf.year, 11, 31); // 31.12.this.year
  weekCount = (lastDayOfYear.getWeek() - 1);

  // If we're going to previous week, and we're stepping between years, be sure to set week and year correct
  if (direction === 'prev') {
    week = parseInt(conf.week, 10) - 1;

    // If week number is less than one, switch to the last week of the
    // previous year.
    if (week < 1) {
      week = weekCount;
      conf.year = conf.year - 1;
    }
  }
  else {
    week = parseInt(conf.week, 10) + 1;

    // If the week number exceeds the total number of weeks for a year,
    // switch to the first week of the next year.
    if (week > weekCount) {
      week = 1;
      conf.year = conf.year + 1;
    }
  }

  $.getJSON(conf.callback + '/' + nid + '/' + conf.field_name + '/' + conf.year + '/' + week, {}, function (data, textStatus) {
    conf.week = parseInt(data.week, 10);
    conf.year = parseInt(data.year, 10);

    $('.office-hours-week.node-' + nid).slideUp('fast', function () {
      $(this).replaceWith(data.html[nid]);
      $('.office-hours-week.node-' + nid).hide().slideDown('fast');
      Drupal.behaviors.officeHours();
    });
  });
  return false;
};

}(jQuery));

