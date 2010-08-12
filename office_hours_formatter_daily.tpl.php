<?php
// $Id$

/**
 * @file
 * Formatter for outputting office hours for a single day.
 */

// For closed days, we have an empty array if closed labels are enabled.
if (empty($day)):?>

<span class="hours closed"><?php print t('closed'); ?></span>

<?php else:
// Otherwise, print each set of hours.
  foreach ($day as $hours):
    if (!empty($hours)):
?>

<span class="hours">
  <span class="open"><?php print office_hours_format_time($hours['open']); ?></span> –
  <span class="close"><?php print office_hours_format_time($hours['close']); ?></span>
</span>

<?php
    endif;
  endforeach;
endif;
?>

