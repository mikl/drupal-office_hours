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
  var nid_matcher = new RegExp("node-(\\d+)");
  $(".office-hours-week:not(.oh-processed)").each(function () {
    $(this);
    var nid = nid_matcher.exec($(this).attr('class'))[1];
    $(this).addClass('oh-processed')
    .find('.week-info')
    .prepend('<a class="prev" href="#">&#9664;</a>')
    .append('<a class="next" href="#">&#9654;</a>')
    .end()
    .find('a.prev')
    .click(function () {return Drupal.officeHours.changeWeek(nid, 'prev');})
    .end()
    .find('a.next')
    .click(function () {return Drupal.officeHours.changeWeek(nid, 'next');})
    .end();
  });
};

// Container object to hold our JavaScript methods.
Drupal.officeHours = {};

// Change view to another week.
Drupal.officeHours.changeWeek = function (nid, direction) {
  var conf = Drupal.settings.officeHours[nid];
  if (direction == 'prev') {
    var week = parseInt(conf.week) - 1;
  }
  else {
    var week = parseInt(conf.week) + 1;
  }

  $.getJSON(conf.callback + '/' + nid + '/' + conf.field_name + '/' + conf.year + '/' + week, {}, function (data, textStatus) {
    Drupal.settings.officeHours[data.nid].week = parseInt(data.week);
    Drupal.settings.officeHours[data.nid].year = parseInt(data.year);

    $('.office-hours-week.node-' + nid).slideUp('normal', function () {
      $(this).replaceWith(data.html);
      $('.office-hours-week.node-' + nid).hide().slideDown();
      Drupal.behaviors.officeHours();
    });
  });
  return false;
};

