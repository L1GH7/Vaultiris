<?php
namespace App\Service;

use Twig\Environment;

class TemplateGenerator
{
    public $template;
    
    private $twig;

    public function __construct($template, Environment $twig)
    {
        $this->template   = $template;
        $this->twig       = $twig;
    }

    public function render()
    {
        return $this->twig->createTemplate($this->template)->render();
    }

}
