import { TestBed } from '@angular/core/testing';

import { EmailValidatorService } from './email-validator.service';

describe('EmailValidatorService', () => {
  beforeEach(() => TestBed.configureTestingModule({}));

  it('should be created', () => {
    const service: EmailValidatorService = TestBed.get(EmailValidatorService);
    expect(service).toBeTruthy();
  });
});
