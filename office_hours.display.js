// $Id$

/**
 * Javascript helpers for the office hours module.
 */

/**
 * Office Hours Drupal JavaScript behaviour
 *
 * attaches the scrolling arrows to the office hours elements.
 */
Drupal.behaviors.officeHours = function () {
  $(".office-hours-week:not(.oh-processed)").each(function () {
    var $this = $(this);
    var match = $this.attr('class').match(Drupal.officeHours.nidMatcher);
    if (match && match[1] > 0) {
      $this.addClass('oh-processed')
        .find('.week-info')
          .prepend('<a class="prev" href="#">&#9664;</a>')
          .append('<a class="next" href="#">&#9654;</a>')
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
 * Helper function to change office hours view to a different week.
 *
 * Calls Drupal's JSON callback to get the new data.
 */
Drupal.officeHours.changeWeek = function (nid, direction) {
  var conf = Drupal.settings.officeHours[nid];
  if (direction == 'prev') {
    var week = parseInt(conf.week) - 1;
  }
  else {
    var week = parseInt(conf.week) + 1;
  }

  $.getJSON(conf.callback + '/' + nid + '/' + conf.field_name + '/' + conf.year + '/' + week, {}, function (data, textStatus) {
    Drupal.settings.officeHours[nid].week = parseInt(data.week);
    Drupal.settings.officeHours[nid].year = parseInt(data.year);

    $('.office-hours-week.node-' + nid).slideUp('fast', function () {
      $(this).replaceWith(data.html[nid]);
      $('.office-hours-week.node-' + nid).hide().slideDown('fast');
      Drupal.behaviors.officeHours();
    });
  });
  return false;
};

