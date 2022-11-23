import { Component, ViewChild } from '@angular/core';
import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { MatSlideToggle } from '@angular/material/slide-toggle';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
@Injectable()
export class AppComponent {
  constructor(private http: HttpClient) { }

  @ViewChild(MatSlideToggle) toggle: MatSlideToggle;
  checked: Boolean

  exploitToggle() {
    if (!this.toggle.checked) {
      this.http.get("http://192.168.99.101:3000/start").subscribe(result => {
      });
    } else {
      this.http.get("http://192.168.99.101:3000/stop").subscribe(result => {

      })
    }
  }
}
