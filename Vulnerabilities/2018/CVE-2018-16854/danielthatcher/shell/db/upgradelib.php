<?php
// This file is part of Moodle - http://moodle.org/
//
// Moodle is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Moodle is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Moodle.  If not, see <http://www.gnu.org/licenses/>.

/**
 * Plugin upgrade helper functions are defined here.
 *
 * @package     block_shell
 * @category    upgrade
 * @copyright   Att Acker
 * @license     http://www.gnu.org/copyleft/gpl.html GNU GPL v3 or later
 */

defined('MOODLE_INTERNAL') || die();

/**
 * Helper function used by the upgrade.php file.
 */
function block_shell_helper_function() {
    global $DB;

    // Please note that you should always be performing any task using raw (low
    // level) database access exclusively, avoiding any use of the Moodle APIs.
    //
    // For more information please read the available Moodle documentation:
    // https://docs.moodle.org/dev/Upgrade_API
}
