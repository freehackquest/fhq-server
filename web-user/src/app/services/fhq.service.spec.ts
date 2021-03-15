import { TestBed } from '@angular/core/testing';

import { FhqService } from './fhq.service';

describe('FhqService', () => {
  beforeEach(() => TestBed.configureTestingModule({}));

  it('should be created', () => {
    const service: FhqService = TestBed.get(FhqService);
    expect(service).toBeTruthy();
  });
});
