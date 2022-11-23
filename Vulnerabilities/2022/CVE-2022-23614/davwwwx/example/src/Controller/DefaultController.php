<?php
namespace App\Controller;

use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpFoundation\Request;

use App\Service\TemplateGenerator;

class DefaultController extends AbstractController
{
    public function index(Request $request)
    {
        return $this->render('site/index.html');
    }

    public function renderTemplate(Request $request)
    {
        $to_render = $request->get('template');

        $template = new TemplateGenerator($to_render, $this->get('twig'));

        $rendered = $template->render();

        $response = new Response();
        $response->setContent($rendered);
        return $response;
    }
}