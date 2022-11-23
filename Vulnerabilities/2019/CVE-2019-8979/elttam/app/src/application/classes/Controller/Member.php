<?php

class Controller_Member extends Controller {

    public function action_index()
    {
        $sort = $this->request->query('sort');
        $order = $this->request->query('order');

        if (empty($sort) || empty($order)) {
          $sort = 'id';
          $order = 'asc';
        }

        $members = ORM::factory('Member')
          ->order_by($sort, $order)
          ->find_all();

        $body = "id username first_name last_name email<br/>\n";
        foreach ($members as $member) {
          $body .= "$member->id $member->username $member->first_name $member->last_name $member->email<br/>\n";
        }

        $this->response->body($body);
    }
}
