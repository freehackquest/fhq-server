import { Injectable } from '@angular/core';
import { L10nTranslationService } from 'angular-l10n';

@Injectable({
  providedIn: 'root'
})
export class EmailValidatorService {

  constructor(
    public _translation: L10nTranslationService,
  ) { }

  checkEmail(email: string) {
    let emailWrongDomains = {};
    // TODO move to server on registration and on login
    emailWrongDomains['yndex.ru'] = {prop: ["yandex.ru"]};
    emailWrongDomains['yadnex.ru'] = {prop: ["yandex.ru"]};
    emailWrongDomains['mil.ru'] = {prop: ["mail.ru"]};
    emailWrongDomains['nail.ru'] = {prop: ["mail.ru"]};
    emailWrongDomains['mial.ru'] = {prop: ["mail.ru"]};
    emailWrongDomains['gmail.ru'] = {prop: ["gmail.com"]};
    emailWrongDomains['gmial.com'] = {prop: ["gmail.com"]};
    emailWrongDomains['gmal.com'] = {prop: ["gmail.com"]};
    emailWrongDomains['inbox.ry'] = {prop: ["inbox.ru"]};
    emailWrongDomains['gamil.com'] = {prop: ["gmail.com"]};

    const re = /^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/;
    let ret = {result: false, error: ""}
    ret.result = re.test(email);
    if (!ret.result) {
      ret.result = false;
      ret.error = "Format email wrong";
      return ret;
    }
    let domain = email.split("@")[1];
    domain = domain.toLowerCase();

    if (emailWrongDomains[domain]) {
      var t = emailWrongDomains[domain];
      ret.result = false;
      ret.error = this._translation.translate('wrongDomainMaybeMeen') + t.prop.join(",");
    }
    return ret;
  }
}
