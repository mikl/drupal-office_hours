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
 * Helper function to change office hours view to a different week.
 *
 * Calls Drupal's JSON callback to get the new data.
 */
Drupal.officeHours.changeWeek = function (nid, direction) {
  var conf, week;
  conf = Drupal.settings.officeHours['node' + nid];
  if (direction == 'prev') {
    week = parseInt(conf.week, 10) - 1;
  }
  else {
    week = parseInt(conf.week, 10) + 1;
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

