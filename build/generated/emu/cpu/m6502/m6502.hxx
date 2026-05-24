void m6502_device::adc_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	do_adc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::adc_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_adc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::adc_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, X)) {
		read(set_l(TMP, TMP+X));
		icount--;
		do_halt();
	}
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += X;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	do_adc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::adc_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_adc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::adc_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
		icount--;
		do_halt();
	}
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += Y;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	do_adc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::adc_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_adc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::adc_idx_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	PC++;
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	do_adc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::adc_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_adc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::adc_idy_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	PC++;
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	}
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	do_adc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::adc_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 7:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	}
	[[fallthrough]];
case 9:
	TMP2 = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_adc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::adc_imm_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	PC++;
	do_adc(TMP);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::adc_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	do_adc(TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::adc_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	do_adc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::adc_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_adc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::adc_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 = read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	do_adc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::adc_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP2 = read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_adc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::and_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::and_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::and_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, X)) {
		read(set_l(TMP, TMP+X));
		icount--;
		do_halt();
	}
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += X;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::and_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::and_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
		icount--;
		do_halt();
	}
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += Y;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::and_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::and_imm_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	A &= TMP;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::and_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	A &= TMP;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::and_idx_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::and_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::and_idy_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	}
	TMP2 = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::and_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 7:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	}
	[[fallthrough]];
case 9:
	TMP2 = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::and_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::and_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::and_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 = read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::and_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP2 = read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::asl_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = do_asl(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::asl_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2 = do_asl(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::asl_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += X;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2 = do_asl(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::asl_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2 = do_asl(TMP2);
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::asl_acc_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	A = do_asl(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::asl_acc_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	A = do_asl(A);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::asl_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP2 = do_asl(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::asl_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP2 = do_asl(TMP2);
	[[fallthrough]];
case 7:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::asl_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = do_asl(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::asl_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2 = do_asl(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::bcc_rel_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	if(!(P & F_C)) {
		branched = true;
		read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
		if(page_changing(PC, int8_t(TMP))) {
			paged = true;
			read_arg(set_l(PC, PC+int8_t(TMP)));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
		}
		PC += int8_t(TMP);
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::bcc_rel_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	if(!(P & F_C)) {
		branched = true;
	[[fallthrough]];
case 3:
		read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(PC, int8_t(TMP))) {
			paged = true;
	[[fallthrough]];
case 5:
			read_arg(set_l(PC, PC+int8_t(TMP)));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
		}
		PC += int8_t(TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::bcs_rel_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	if(P & F_C) {
		branched = true;
		read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
		if(page_changing(PC, int8_t(TMP))) {
			paged = true;
			read_arg(set_l(PC, PC+int8_t(TMP)));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
		}
		PC += int8_t(TMP);
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::bcs_rel_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	if(P & F_C) {
		branched = true;
	[[fallthrough]];
case 3:
		read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(PC, int8_t(TMP))) {
			paged = true;
	[[fallthrough]];
case 5:
			read_arg(set_l(PC, PC+int8_t(TMP)));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
		}
		PC += int8_t(TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::beq_rel_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	if(P & F_Z) {
		branched = true;
		read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
		if(page_changing(PC, int8_t(TMP))) {
			paged = true;
			read_arg(set_l(PC, PC+int8_t(TMP)));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
		}
		PC += int8_t(TMP);
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::beq_rel_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	if(P & F_Z) {
		branched = true;
	[[fallthrough]];
case 3:
		read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(PC, int8_t(TMP))) {
			paged = true;
	[[fallthrough]];
case 5:
			read_arg(set_l(PC, PC+int8_t(TMP)));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
		}
		PC += int8_t(TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::bit_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP2 = read(TMP);
	do_bit(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
		prefetch_end();
}

void m6502_device::bit_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_bit(TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::bit_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else {
			inst_substate = 4;
		}
		return;
	}
	do_bit(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else {
			inst_substate = 6;
		}
		return;
	}
	prefetch_end();
}

void m6502_device::bit_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_bit(TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::bmi_rel_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	if(P & F_N) {
		branched = true;
		read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
		if(page_changing(PC, int8_t(TMP))) {
			paged = true;
			read_arg(set_l(PC, PC+int8_t(TMP)));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
		}
		PC += int8_t(TMP);
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::bmi_rel_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	if(P & F_N) {
		branched = true;
	[[fallthrough]];
case 3:
		read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(PC, int8_t(TMP))) {
			paged = true;
	[[fallthrough]];
case 5:
			read_arg(set_l(PC, PC+int8_t(TMP)));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
		}
		PC += int8_t(TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::bne_rel_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	if(!(P & F_Z)) {
		branched = true;
		read_pc();
		icount--;
		do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
		if(page_changing(PC, int8_t(TMP))) {
			paged = true;
			read_arg(set_l(PC, PC+int8_t(TMP)));
			icount--;
			do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
		}
		PC += int8_t(TMP);
	}
		prefetch_start();
		IR = read_sync(PC);
		icount--;
		do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();	
}

void m6502_device::bne_rel_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	if(!(P & F_Z)) {
		branched = true;
	[[fallthrough]];
case 3:
		read_pc();
	icount--;
		do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(PC, int8_t(TMP))) {
			paged = true;
	[[fallthrough]];
case 5:
			read_arg(set_l(PC, PC+int8_t(TMP)));
	icount--;
				do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
		}
		PC += int8_t(TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::bpl_rel_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	if(!(P & F_N)) {
		branched = true;
		read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
		if(page_changing(PC, int8_t(TMP))) {
			paged = true;
			read_arg(set_l(PC, PC+int8_t(TMP)));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
		}
		PC += int8_t(TMP);
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::bpl_rel_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	if(!(P & F_N)) {
		branched = true;
	[[fallthrough]];
case 3:
		read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(PC, int8_t(TMP))) {
			paged = true;
	[[fallthrough]];
case 5:
			read_arg(set_l(PC, PC+int8_t(TMP)));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
		}
		PC += int8_t(TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::brk_imp_full()
{
	// The 6502 bug when a nmi occurs in a brk is reproduced (case !irq_taken && nmi_pending)
	if(irq_taken) {
		read_pc();
		next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	} else {
		read_pc();
		next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
		PC++;
	}
	write(SP, PC >> 8);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	dec_SP();
	write_1(SP, PC);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	dec_SP();
	write(SP, irq_taken ? ((P & ~F_B) | F_T) : (P | F_B | F_T));
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	dec_SP();
	if(nmi_pending) {
		standard_irq_callback(NMI_LINE, PC);
		PC = read_arg(0xfffa);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
		PC = set_h(PC, read_arg(0xfffb));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
		nmi_pending = false;
	} else {
		if(irq_taken)
			standard_irq_callback(IRQ_LINE, PC);
		PC = read_arg(0xfffe);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
		PC = set_h(PC, read_arg(0xffff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 17;
		} else
			inst_substate = 18;
		return;
	}
	}
	irq_taken = false;
	P |= F_I; // Do *not* move after the prefetch
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 19;
		} else
			inst_substate = 20;
		return;
	}
	prefetch_end();
	inst_state = -1;
}

void m6502_device::brk_imp_partial()
{
	switch(inst_substate) {
case 0:
	// The 6502 bug when a nmi occurs in a brk is reproduced (case !irq_taken && nmi_pending)
	if(irq_taken) {
	[[fallthrough]];
case 1:
		read_pc();
		next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	} else {
	[[fallthrough]];
case 3:
		read_pc();
		next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		PC++;
	}
	[[fallthrough]];
case 5:
	write(SP, PC >> 8);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	dec_SP();
	[[fallthrough]];
case 7:
	write_1(SP, PC);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	dec_SP();
	[[fallthrough]];
case 9:
	write(SP, irq_taken ? ((P & ~F_B) | F_T) : (P | F_B | F_T));
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	dec_SP();
	if(nmi_pending) {
		standard_irq_callback(NMI_LINE, PC);
	[[fallthrough]];
case 11:
		PC = read_arg(0xfffa);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	[[fallthrough]];
case 13:
		PC = set_h(PC, read_arg(0xfffb));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
		nmi_pending = false;
	} else {
		if(irq_taken)
			standard_irq_callback(IRQ_LINE, PC);
	[[fallthrough]];
case 15:
		PC = read_arg(0xfffe);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	[[fallthrough]];
case 17:
		PC = set_h(PC, read_arg(0xffff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 17;
		} else
			inst_substate = 18;
		return;
	}
	[[fallthrough]];
case 18:;
	}
	irq_taken = false;
	P |= F_I; // Do *not* move after the prefetch
	prefetch_start();
	[[fallthrough]];
case 19:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 19;
		} else
			inst_substate = 20;
		return;
	}
	[[fallthrough]];
case 20:;
	prefetch_end();
	inst_state = -1;
	break;
}
	inst_substate = 0;
}

void m6502_device::bvc_rel_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	if(!(P & F_V)) {
		branched = true;
		read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
		if(page_changing(PC, int8_t(TMP))) {
			paged = true;
			read_arg(set_l(PC, PC+int8_t(TMP)));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
		}
		PC += int8_t(TMP);
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::bvc_rel_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	if(!(P & F_V)) {
		branched = true;
	[[fallthrough]];
case 3:
		read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(PC, int8_t(TMP))) {
			paged = true;
	[[fallthrough]];
case 5:
			read_arg(set_l(PC, PC+int8_t(TMP)));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
		}
		PC += int8_t(TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::bvs_rel_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	if(P & F_V) {
		branched = true;
		read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
		if(page_changing(PC, int8_t(TMP))) {
			paged = true;
			read_arg(set_l(PC, PC+int8_t(TMP)));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
		}
		PC += int8_t(TMP);
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::bvs_rel_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	if(P & F_V) {
		branched = true;
	[[fallthrough]];
case 3:
		read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(PC, int8_t(TMP))) {
			paged = true;
	[[fallthrough]];
case 5:
			read_arg(set_l(PC, PC+int8_t(TMP)));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
		}
		PC += int8_t(TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::clc_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	P &= ~F_C;
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::clc_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	P &= ~F_C;
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::cld_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	P &= ~F_D;
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::cld_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	P &= ~F_D;
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::cli_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
	P &= ~F_I; // Do *not* move it before the prefetch
}

void m6502_device::cli_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	P &= ~F_I; // Do *not* move it before the prefetch
	break;
}
	inst_substate = 0;
}

void m6502_device::clv_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	P &= ~F_V;
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::clv_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	P &= ~F_V;
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::cmp_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	do_cmp(A, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::cmp_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_cmp(A, TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::cmp_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, X)) {
		read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	}
	TMP += X;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	do_cmp(A, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::cmp_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_cmp(A, TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::cmp_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	}
	TMP += Y;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	do_cmp(A, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::cmp_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_cmp(A, TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::cmp_idx_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	do_cmp(A, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::cmp_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_cmp(A, TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::cmp_idy_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	}
	TMP2 = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	do_cmp(A, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::cmp_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 7:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	}
	[[fallthrough]];
case 9:
	TMP2 = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_cmp(A, TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::cmp_imm_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	do_cmp(A, TMP);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::cmp_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	do_cmp(A, TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::cmp_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	do_cmp(A, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::cmp_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_cmp(A, TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::cmp_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 = read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	do_cmp(A, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::cmp_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP2 = read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_cmp(A, TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::cpx_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	do_cmp(X, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::cpx_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_cmp(X, TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::cpx_imm_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	do_cmp(X, TMP);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::cpx_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	do_cmp(X, TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::cpx_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	do_cmp(X, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::cpx_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_cmp(X, TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::cpy_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	do_cmp(Y, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::cpy_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_cmp(Y, TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::cpy_imm_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	do_cmp(Y, TMP);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::cpy_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	do_cmp(Y, TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::cpy_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	do_cmp(Y, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::cpy_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_cmp(Y, TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::dec_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2--;
	set_nz(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::dec_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2--;
	set_nz(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::dec_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += X;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2--;
	set_nz(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::dec_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2--;
	set_nz(TMP2);
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::dec_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP2--;
	set_nz(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::dec_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP2--;
	set_nz(TMP2);
	[[fallthrough]];
case 7:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::dec_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2--;
	set_nz(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::dec_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2--;
	set_nz(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::dex_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	X--;
	set_nz(X);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::dex_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	X--;
	set_nz(X);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::dey_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	Y--;
	set_nz(Y);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::dey_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	Y--;
	set_nz(Y);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::eor_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::eor_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::eor_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, X)) {
		read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	}
	TMP += X;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::eor_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::eor_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	}
	TMP += Y;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::eor_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::eor_idx_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::eor_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::eor_idy_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	}
	TMP2 = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::eor_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 7:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	}
	[[fallthrough]];
case 9:
	TMP2 = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::eor_imm_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	A ^= TMP;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::eor_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	A ^= TMP;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::eor_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::eor_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::eor_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 = read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::eor_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP2 = read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::inc_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2++;
	set_nz(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::inc_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2++;
	set_nz(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::inc_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += X;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2++;
	set_nz(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::inc_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2++;
	set_nz(TMP2);
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::inc_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP2++;
	set_nz(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::inc_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP2++;
	set_nz(TMP2);
	[[fallthrough]];
case 7:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::inc_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2++;
	set_nz(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::inc_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2++;
	set_nz(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::inx_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	X++;
	set_nz(X);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::inx_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	X++;
	set_nz(X);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::iny_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	Y++;
	set_nz(Y);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::iny_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	Y++;
	set_nz(Y);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::jmp_adr_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC = TMP;
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::jmp_adr_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC = TMP;
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::jmp_ind_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	PC = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	PC = set_h(PC, read(set_l(TMP, TMP+1)));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::jmp_ind_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	PC = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	PC = set_h(PC, read(set_l(TMP, TMP+1)));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::jsr_adr_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	PC++;
	read(SP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	write_1(SP, PC>>8);
	dec_SP();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write(SP, PC);
	next_read = true;
	dec_SP();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else {
			inst_substate = 10;
		}
		return;
	}
	PC = TMP;
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else {
			inst_substate = 12;
		}
		return;
	}
	prefetch_end();
}

void m6502_device::jsr_adr_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(SP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write_1(SP, PC>>8);
	dec_SP();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(SP, PC);
	next_read = true;
	dec_SP();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	PC = TMP;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lda_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	A = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::lda_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	A = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lda_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, X)) {
		read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	}
	A = read(TMP + X);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::lda_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	[[fallthrough]];
case 7:
	A = read(TMP + X);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lda_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	}
	A = read(TMP + Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::lda_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	[[fallthrough]];
case 7:
	A = read(TMP + Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lda_idx_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	A = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::lda_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	A = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lda_idy_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	}
	A = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::lda_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 7:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	}
	[[fallthrough]];
case 9:
	A = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lda_imm_full()
{
	A = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	PC++;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else {
			inst_substate = 4;
		}
		return;
	}
	prefetch_end();
}

void m6502_device::lda_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	A = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lda_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	A = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::lda_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	A = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lda_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	A = read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::lda_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	A = read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ldx_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	X = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	set_nz(X);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::ldx_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	X = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(X);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ldx_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	}
	X = read(TMP + Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	set_nz(X);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::ldx_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	[[fallthrough]];
case 7:
	X = read(TMP + Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	set_nz(X);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ldx_imm_full()
{
	X = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	set_nz(X);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::ldx_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	X = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	set_nz(X);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ldx_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	X = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	set_nz(X);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::ldx_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	X = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	set_nz(X);
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ldx_zpy_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	X = read(uint8_t(TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	set_nz(X);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::ldx_zpy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	X = read(uint8_t(TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(X);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ldy_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	Y = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	set_nz(Y);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::ldy_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	Y = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(Y);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ldy_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, X)) {
		read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	}
	TMP += X;
	Y = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	set_nz(Y);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::ldy_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	TMP += X;
	[[fallthrough]];
case 7:
	Y = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	set_nz(Y);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ldy_imm_full()
{
	Y = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	set_nz(Y);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::ldy_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	Y = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	set_nz(Y);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ldy_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	Y = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	set_nz(Y);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::ldy_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	Y = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	set_nz(Y);
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ldy_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	Y = read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	set_nz(Y);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::ldy_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	Y = read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(Y);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lsr_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = do_lsr(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::lsr_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2 = do_lsr(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lsr_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += X;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2 = do_lsr(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::lsr_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2 = do_lsr(TMP2);
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lsr_acc_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	A = do_lsr(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::lsr_acc_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	A = do_lsr(A);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lsr_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP2 = do_lsr(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::lsr_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP2 = do_lsr(TMP2);
	[[fallthrough]];
case 7:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lsr_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = do_lsr(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::lsr_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2 = do_lsr(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::nop_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::nop_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ora_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::ora_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ora_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, X)) {
		read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	}
	TMP += X;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::ora_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ora_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	}
	TMP += Y;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::ora_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ora_imm_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	A |= TMP;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::ora_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	A |= TMP;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ora_idx_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::ora_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ora_idy_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	}
	TMP2 = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::ora_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 7:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	}
	[[fallthrough]];
case 9:
	TMP2 = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ora_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::ora_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ora_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 = read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::ora_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP2 = read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::pha_imp_full()
{
	read_pc();
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	write(SP, A);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else {
			inst_substate = 4;
		}
		return;
	}
	dec_SP();
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else {
			inst_substate = 6;
		}
		return;
	}
	prefetch_end();
}

void m6502_device::pha_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	write(SP, A);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	dec_SP();
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::php_imp_full()
{
	read_pc();
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	write(SP, (P | F_B | F_T));
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	dec_SP();
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::php_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	write(SP, (P | F_B | F_T));
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	dec_SP();
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::pla_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	inc_SP();
	A = read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::pla_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	inc_SP();
	[[fallthrough]];
case 5:
	A = read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::plp_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	inc_SP();
	TMP = read(SP) & 0xCF;// | (F_B|F_E);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
	P = TMP; // Do *not* move it before the prefetch
}

void m6502_device::plp_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	inc_SP();
	[[fallthrough]];
case 5:
	TMP = read(SP) & 0xCF;// | (F_B|F_E);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	P = TMP; // Do *not* move it before the prefetch
	break;
}
	inst_substate = 0;
}

void m6502_device::rol_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = do_rol(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::rol_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2 = do_rol(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rol_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += X;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2 = do_rol(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::rol_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2 = do_rol(TMP2);
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rol_acc_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	A = do_rol(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::rol_acc_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	A = do_rol(A);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rol_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP2 = do_rol(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::rol_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP2 = do_rol(TMP2);
	[[fallthrough]];
case 7:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rol_zpx_full()
{
	TMP = read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = do_rol(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::rol_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2 = do_rol(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ror_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = do_ror(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::ror_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2 = do_ror(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ror_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += X;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2 = do_ror(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::ror_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2 = do_ror(TMP2);
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ror_acc_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	A = do_ror(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::ror_acc_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	A = do_ror(A);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ror_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP2 = do_ror(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::ror_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP2 = do_ror(TMP2);
	[[fallthrough]];
case 7:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ror_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = do_ror(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::ror_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2 = do_ror(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rti_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	inc_SP();
	P = read(SP) & 0xCF; //| (F_B|F_E); ////
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	inc_SP();
	PC = read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	inc_SP();
	PC = set_h(PC, read(SP));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::rti_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	inc_SP();
	[[fallthrough]];
case 5:
	P = read(SP) & 0xCF;// | (F_B|F_E);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	inc_SP();
	[[fallthrough]];
case 7:
	PC = read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	inc_SP();
	[[fallthrough]];
case 9:
	PC = set_h(PC, read(SP));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rts_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	inc_SP();
	PC = read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	inc_SP();
	PC = set_h(PC, read(SP));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	PC++;
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::rts_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	inc_SP();
	[[fallthrough]];
case 5:
	PC = read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	inc_SP();
	[[fallthrough]];
case 7:
	PC = set_h(PC, read(SP));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	PC++;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sbc_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	do_sbc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::sbc_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_sbc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sbc_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, X)) {
		read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	}
	TMP += X;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	do_sbc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::sbc_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_sbc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sbc_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	}
	TMP += Y;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	do_sbc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::sbc_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_sbc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sbc_idx_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	do_sbc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::sbc_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_sbc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sbc_idy_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	}
	TMP2 = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	do_sbc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::sbc_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 7:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	}
	[[fallthrough]];
case 9:
	TMP2 = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_sbc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sbc_imm_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	do_sbc(TMP);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::sbc_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	do_sbc(TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sbc_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	do_sbc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::sbc_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_sbc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sbc_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 = read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	do_sbc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::sbc_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP2 = read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_sbc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sec_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	P |= F_C;
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else {
			inst_substate = 4;
		}
		return;
	}
	prefetch_end();
}

void m6502_device::sec_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	P |= F_C;
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sed_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	P |= F_D;
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::sed_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	P |= F_D;
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sei_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
	P |= F_I; // Do *not* move it before the prefetch
}

void m6502_device::sei_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	P |= F_I; // Do *not* move it before the prefetch
	break;
}
	inst_substate = 0;
}

void m6502_device::sta_aba_full()
{	
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else {
			inst_substate = 2;
		}
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	write(TMP, A);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else {
			inst_substate = 6;
		}
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else {
			inst_substate = 8;
		}
		return;
	}
	prefetch_end();
}

void m6502_device::sta_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	write(TMP, A);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sta_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+X));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write(TMP+X, A);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::sta_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+X));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(TMP+X, A);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sta_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+Y));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write(TMP+Y, A);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::sta_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+Y));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(TMP+Y, A);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sta_idx_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write(TMP, A);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::sta_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(TMP, A);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sta_idy_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	read(set_l(TMP, TMP+Y));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write(TMP+Y, A);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::sta_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(TMP, TMP+Y));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(TMP+Y, A);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sta_zpg_full()
{
	TMP = read_pc();
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	write(TMP, A);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::sta_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	write(TMP, A);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sta_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	write(uint8_t(TMP+X), A);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::sta_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(uint8_t(TMP+X), A);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::stx_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	write(TMP, X);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::stx_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	write(TMP, X);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::stx_zpg_full()
{
	TMP = read_pc();
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	write(TMP, X);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::stx_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	write(TMP, X);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::stx_zpy_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	write(uint8_t(TMP+Y), X);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::stx_zpy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(uint8_t(TMP+Y), X);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sty_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	write(TMP, Y);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::sty_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	write(TMP, Y);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sty_zpg_full()
{
	TMP = read_pc();
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	write(TMP, Y);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::sty_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	write(TMP, Y);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sty_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	write(uint8_t(TMP+X), Y);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::sty_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(uint8_t(TMP+X), Y);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::tax_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	X = A;
	set_nz(X);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::tax_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	X = A;
	set_nz(X);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::tay_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	Y = A;
	set_nz(Y);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::tay_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	Y = A;
	set_nz(Y);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::tsx_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	X = SP;
	set_nz(X);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::tsx_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	X = SP;
	set_nz(X);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::txa_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	A = X;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::txa_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	A = X;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::txs_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	SP = set_l(SP, X);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::txs_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	SP = set_l(SP, X);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::tya_imp_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	A = Y;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::tya_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	A = Y;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::reset_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	dec_SP();
	read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	dec_SP();
	read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	dec_SP();
	P &= ~F_B;
	P |= F_I;
	PC = read_arg(0xfffc);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	PC = set_h(PC, read_arg(0xfffd));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	prefetch_end();
	inst_state = -1;
}

void m6502_device::reset_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	dec_SP();
	[[fallthrough]];
case 7:
	read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	dec_SP();
	[[fallthrough]];
case 9:
	read(SP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:
	dec_SP();
	P &= ~F_B;
	P |= F_I;
	[[fallthrough]];
case 11:
	PC = read_arg(0xfffc);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	[[fallthrough]];
case 13:
	PC = set_h(PC, read_arg(0xfffd));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_start();
	[[fallthrough]];
case 15:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	inst_state = -1;
	break;
}
	inst_substate = 0;
}

void m6502_device::dcp_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2--;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	do_cmp(A, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::dcp_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2--;
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_cmp(A, TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::dcp_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += X;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2--;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	do_cmp(A, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::dcp_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2--;
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_cmp(A, TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::dcp_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += Y;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2--;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	do_cmp(A, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::dcp_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2--;
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_cmp(A, TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::dcp_idx_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	TMP2--;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	do_cmp(A, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::dcp_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	TMP2--;
	[[fallthrough]];
case 13:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_cmp(A, TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::dcp_idy_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP += Y;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	TMP2--;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	do_cmp(A, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::dcp_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP += Y;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	TMP2--;
	[[fallthrough]];
case 13:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_cmp(A, TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::dcp_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP2--;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	do_cmp(A, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::dcp_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP2--;
	[[fallthrough]];
case 7:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_cmp(A, TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::dcp_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2--;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	do_cmp(A, TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::dcp_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2--;
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_cmp(A, TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::isb_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2++;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	do_sbc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::isb_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2++;
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_sbc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::isb_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += X;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2++;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	do_sbc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::isb_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2++;
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_sbc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::isb_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += Y;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2++;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	do_sbc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::isb_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2++;
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_sbc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::isb_idx_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	TMP2++;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	do_sbc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::isb_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	TMP2++;
	[[fallthrough]];
case 13:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_sbc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::isb_idy_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP += Y;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	TMP2++;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	do_sbc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::isb_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP += Y;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	TMP2++;
	[[fallthrough]];
case 13:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_sbc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::isb_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP2++;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	do_sbc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::isb_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP2++;
	[[fallthrough]];
case 7:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_sbc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::isb_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2++;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	do_sbc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::isb_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2++;
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_sbc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lax_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	A = X = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::lax_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	A = X = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lax_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	}
	A = X = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::lax_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	[[fallthrough]];
case 7:
	A = X = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lax_idx_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	A = X = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::lax_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	A = X = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lax_idy_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	}
	A = X = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::lax_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 7:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	}
	[[fallthrough]];
case 9:
	A = X = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lax_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	A = X = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::lax_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	A = X = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lax_zpy_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = uint8_t(TMP+Y);
	A = X = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::lax_zpy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP = uint8_t(TMP+Y);
	[[fallthrough]];
case 5:
	A = X = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rla_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = do_rol(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::rla_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2 = do_rol(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rla_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += X;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2 = do_rol(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::rla_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2 = do_rol(TMP2);
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rla_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += Y;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2 = do_rol(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::rla_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2 = do_rol(TMP2);
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rla_idx_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	TMP2 = do_rol(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::rla_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	TMP2 = do_rol(TMP2);
	[[fallthrough]];
case 13:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 15:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rla_idy_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP += Y;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	TMP2 = do_rol(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::rla_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP += Y;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	TMP2 = do_rol(TMP2);
	[[fallthrough]];
case 13:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 15:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rla_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP2 = do_rol(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::rla_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP2 = do_rol(TMP2);
	[[fallthrough]];
case 7:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rla_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = do_rol(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::rla_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2 = do_rol(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rra_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = do_ror(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	do_adc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::rra_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2 = do_ror(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_adc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rra_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += X;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2 = do_ror(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	do_adc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::rra_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2 = do_ror(TMP2);
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_adc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rra_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += Y;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2 = do_ror(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	do_adc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::rra_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2 = do_ror(TMP2);
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_adc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rra_idx_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	write(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	TMP2 = do_ror(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	do_adc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::rra_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	TMP2 = do_ror(TMP2);
	[[fallthrough]];
case 13:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_adc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rra_idy_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP += Y;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	write(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	TMP2 = do_ror(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	do_adc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::rra_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP += Y;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	TMP2 = do_ror(TMP2);
	[[fallthrough]];
case 13:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_adc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rra_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP2 = do_ror(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	do_adc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::rra_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP2 = do_ror(TMP2);
	[[fallthrough]];
case 7:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_adc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::rra_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = do_ror(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	do_adc(TMP2);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::rra_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2 = do_ror(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_adc(TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sax_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = A & X;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::sax_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	TMP2 = A & X;
	[[fallthrough]];
case 5:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sax_idx_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = A & X;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::sax_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2 = A & X;
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sax_zpg_full()
{
	TMP = read_pc();
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = A & X;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::sax_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	next_read = false;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	TMP2 = A & X;
	[[fallthrough]];
case 3:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sax_zpy_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = uint8_t(TMP+Y);
	TMP2 = A & X;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::sax_zpy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP = uint8_t(TMP+Y);
	TMP2 = A & X;
	[[fallthrough]];
case 5:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sbx_imm_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	X &= A;
	if(X < TMP2)
		P &= ~F_C;
	else
		P |= F_C;
	X -= TMP2;
	set_nz(X);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::sbx_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	X &= A;
	if(X < TMP2)
		P &= ~F_C;
	else
		P |= F_C;
	X -= TMP2;
	set_nz(X);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sha_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+Y));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(inst_halted) {
		TMP2 = A & X;
	} else {
		TMP2 = A & X & ((TMP >> 8)+1);
	}
	if(page_changing(TMP, Y))
		TMP = set_h(TMP+Y, TMP2);
	else
		TMP += Y;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::sha_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+Y));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(inst_halted) {
		TMP2 = A & X;
	} else {
		TMP2 = A & X & ((TMP >> 8)+1);
	}
	if(page_changing(TMP, Y))
		TMP = set_h(TMP+Y, TMP2);
	else
		TMP += Y;
	[[fallthrough]];
case 7:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sha_idy_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	read(set_l(TMP, TMP+Y));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	if(inst_halted) {
		TMP2 = A & X;
	} else {
		TMP2 = A & X & ((TMP >> 8)+1);
	}
	if(page_changing(TMP, Y))
		TMP = set_h(TMP+Y, TMP2);
	else
		TMP += Y;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::sha_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(TMP, TMP+Y));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	if(inst_halted) {
		TMP2 = A & X;
	} else {
		TMP2 = A & X & ((TMP >> 8)+1);
	}
	if(page_changing(TMP, Y))
		TMP = set_h(TMP+Y, TMP2);
	else
		TMP += Y;
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::shs_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+Y));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	SP = set_l(SP, A & X);
	if(inst_halted) {
		TMP2 = A & X;
	} else {
		TMP2 = A & X & ((TMP >> 8)+1);
	}
	if(page_changing(TMP, Y))
		TMP = set_h(TMP+Y, TMP2);
	else
		TMP += Y;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::shs_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+Y));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	SP = set_l(SP, A & X);
	if(inst_halted) {
		TMP2 = A & X;
	} else {
		TMP2 = A & X & ((TMP >> 8)+1);
	}
	if(page_changing(TMP, Y))
		TMP = set_h(TMP+Y, TMP2);
	else
		TMP += Y;
	[[fallthrough]];
case 7:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::shx_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+Y));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(inst_halted) {
		TMP2 = X;
	} else {
		TMP2 = X & ((TMP >> 8)+1);
	}
	if(page_changing(TMP, Y))
		TMP = set_h(TMP+Y, TMP2);
	else
		TMP += Y;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::shx_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+Y));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(inst_halted) {
		TMP2 = X;
	} else {
		TMP2 = X & ((TMP >> 8)+1);
	}
	if(page_changing(TMP, Y))
		TMP = set_h(TMP+Y, TMP2);
	else
		TMP += Y;
	[[fallthrough]];
case 7:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::shy_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+X));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(inst_halted) {
		TMP2 = Y;
	} else {
		TMP2 = Y & ((TMP >> 8)+1);
	}
	if(page_changing(TMP, X))
		TMP = set_h(TMP+X, TMP2);
	else
		TMP += X;
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::shy_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+X));
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(inst_halted) {
		TMP2 = Y;
	} else {
		TMP2 = Y & ((TMP >> 8)+1);
	}
	if(page_changing(TMP, X))
		TMP = set_h(TMP+X, TMP2);
	else
		TMP += X;
	[[fallthrough]];
case 7:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::slo_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	next_read = false;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = do_asl(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::slo_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2 = do_asl(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::slo_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += X;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2 = do_asl(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::slo_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2 = do_asl(TMP2);
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::slo_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += Y;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2 = do_asl(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::slo_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	icount--;
	next_read = false;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2 = do_asl(TMP2);
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::slo_idx_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = read(TMP);
	icount--;
	next_read = false;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	TMP2 = do_asl(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::slo_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	TMP2 = do_asl(TMP2);
	[[fallthrough]];
case 13:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 15:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::slo_idy_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP += Y;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	TMP2 = do_asl(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::slo_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP += Y;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	icount--;
	next_read = false;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	TMP2 = do_asl(TMP2);
	[[fallthrough]];
case 13:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 15:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::slo_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP2 = do_asl(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::slo_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP2 = do_asl(TMP2);
	[[fallthrough]];
case 7:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::slo_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = do_asl(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::slo_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	icount--;
	next_read = false;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2 = do_asl(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sre_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	next_read = false;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = do_lsr(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::sre_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2 = do_lsr(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sre_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += X;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2 = do_lsr(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::sre_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2 = do_lsr(TMP2);
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sre_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += Y;
	TMP2 = read(TMP);
	icount--;
	next_read = false;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	TMP2 = do_lsr(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_end();
}

void m6502_device::sre_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
	icount--;
	next_read = false;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	TMP2 = do_lsr(TMP2);
	[[fallthrough]];
case 11:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sre_idx_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = read(TMP);
	icount--;
	next_read = false;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	TMP2 = do_lsr(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::sre_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	icount--;
	next_read = false;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	TMP2 = do_lsr(TMP2);
	[[fallthrough]];
case 13:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 15:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sre_idy_full()
{
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP += Y;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	TMP2 = do_lsr(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	prefetch_end();
}

void m6502_device::sre_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = read(TMP2);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP += Y;
	[[fallthrough]];
case 9:
	TMP2 = read(TMP);
	icount--;
	next_read = false;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	TMP2 = do_lsr(TMP2);
	[[fallthrough]];
case 13:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 15:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sre_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP2 = do_lsr(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::sre_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
	icount--;
	next_read = false;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	TMP2 = do_lsr(TMP2);
	[[fallthrough]];
case 7:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::sre_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	next_read = false;
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	TMP2 = do_lsr(TMP2);
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	prefetch_end();
}

void m6502_device::sre_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
	icount--;
	next_read = false;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write_1(TMP, TMP2);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	TMP2 = do_lsr(TMP2);
	[[fallthrough]];
case 9:
	write(TMP, TMP2);
	next_read = true;
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::anc_imm_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	A &= TMP;
	set_nz(A);
	if(A & 0x80)
		P |= F_C;
	else
		P &= ~F_C;
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::anc_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	A &= TMP;
	set_nz(A);
	if(A & 0x80)
		P |= F_C;
	else
		P &= ~F_C;
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::ane_imm_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	A &= TMP & X;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::ane_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	A &= TMP & X;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::asr_imm_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	A = do_lsr(A & TMP);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::asr_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	A = do_lsr(A & TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::arr_imm_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	A &= TMP;
	do_arr();
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::arr_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	A &= TMP;
	do_arr();
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::las_aby_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, Y)) {
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	}
	TMP2 = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	A = TMP2 & SP;
	X = A;
	SP = set_l(SP, A);
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::las_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+Y));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	[[fallthrough]];
case 7:
	TMP2 = read(TMP+Y);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	A = TMP2 & SP;
	X = A;
	SP = set_l(SP, A);
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::lxa_imm_full()
{
	A = X = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	set_nz(A);
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::lxa_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	A = X = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::nop_aba_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::nop_aba_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	[[fallthrough]];
case 5:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::nop_abx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	if(page_changing(TMP, X)) {
		read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	}
	read(TMP + X);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	prefetch_end();
}

void m6502_device::nop_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	PC++;
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read(set_l(TMP, TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	[[fallthrough]];
case 7:
	read(TMP + X);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::nop_imm_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_end();
}

void m6502_device::nop_imm_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	prefetch_start();
	[[fallthrough]];
case 3:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::nop_zpg_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_end();
}

void m6502_device::nop_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_start();
	[[fallthrough]];
case 5:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::nop_zpx_full()
{
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	prefetch_start();
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	prefetch_end();
}

void m6502_device::nop_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP = read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(TMP);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	read(uint8_t(TMP+X));
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = read_sync(PC);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void m6502_device::kil_non_full()
{
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	PC++;
	read(0xffff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	read(0xfffe);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	read(0xfffe);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	for(;;) {
		read(0xffff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	}
}

void m6502_device::kil_non_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	read(0xffff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	read(0xfffe);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(0xfffe);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	for(;;) {
	[[fallthrough]];
case 9:
		read(0xffff);
	icount--;
	do_halt();
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	}
	break;
}
	inst_substate = 0;
}



void m6502_device::do_exec_full()
{
	switch(inst_state) {

	case 0x00: brk_imp_full(); break;
	case 0x01: ora_idx_full(); break;
	case 0x02: kil_non_full(); break;
	case 0x03: slo_idx_full(); break;
	case 0x04: nop_zpg_full(); break;
	case 0x05: ora_zpg_full(); break;
	case 0x06: asl_zpg_full(); break;
	case 0x07: slo_zpg_full(); break;
	case 0x08: php_imp_full(); break;
	case 0x09: ora_imm_full(); break;
	case 0x0a: asl_acc_full(); break;
	case 0x0b: anc_imm_full(); break;
	case 0x0c: nop_aba_full(); break;
	case 0x0d: ora_aba_full(); break;
	case 0x0e: asl_aba_full(); break;
	case 0x0f: slo_aba_full(); break;
	case 0x10: bpl_rel_full(); break;
	case 0x11: ora_idy_full(); break;
	case 0x12: kil_non_full(); break;
	case 0x13: slo_idy_full(); break;
	case 0x14: nop_zpx_full(); break;
	case 0x15: ora_zpx_full(); break;
	case 0x16: asl_zpx_full(); break;
	case 0x17: slo_zpx_full(); break;
	case 0x18: clc_imp_full(); break;
	case 0x19: ora_aby_full(); break;
	case 0x1a: nop_imp_full(); break;
	case 0x1b: slo_aby_full(); break;
	case 0x1c: nop_abx_full(); break;
	case 0x1d: ora_abx_full(); break;
	case 0x1e: asl_abx_full(); break;
	case 0x1f: slo_abx_full(); break;
	case 0x20: jsr_adr_full(); break;
	case 0x21: and_idx_full(); break;
	case 0x22: kil_non_full(); break;
	case 0x23: rla_idx_full(); break;
	case 0x24: bit_zpg_full(); break;
	case 0x25: and_zpg_full(); break;
	case 0x26: rol_zpg_full(); break;
	case 0x27: rla_zpg_full(); break;
	case 0x28: plp_imp_full(); break;
	case 0x29: and_imm_full(); break;
	case 0x2a: rol_acc_full(); break;
	case 0x2b: anc_imm_full(); break;
	case 0x2c: bit_aba_full(); break;
	case 0x2d: and_aba_full(); break;
	case 0x2e: rol_aba_full(); break;
	case 0x2f: rla_aba_full(); break;
	case 0x30: bmi_rel_full(); break;
	case 0x31: and_idy_full(); break;
	case 0x32: kil_non_full(); break;
	case 0x33: rla_idy_full(); break;
	case 0x34: nop_zpx_full(); break;
	case 0x35: and_zpx_full(); break;
	case 0x36: rol_zpx_full(); break;
	case 0x37: rla_zpx_full(); break;
	case 0x38: sec_imp_full(); break;
	case 0x39: and_aby_full(); break;
	case 0x3a: nop_imp_full(); break;
	case 0x3b: rla_aby_full(); break;
	case 0x3c: nop_abx_full(); break;
	case 0x3d: and_abx_full(); break;
	case 0x3e: rol_abx_full(); break;
	case 0x3f: rla_abx_full(); break;
	case 0x40: rti_imp_full(); break;
	case 0x41: eor_idx_full(); break;
	case 0x42: kil_non_full(); break;
	case 0x43: sre_idx_full(); break;
	case 0x44: nop_zpg_full(); break;
	case 0x45: eor_zpg_full(); break;
	case 0x46: lsr_zpg_full(); break;
	case 0x47: sre_zpg_full(); break;
	case 0x48: pha_imp_full(); break;
	case 0x49: eor_imm_full(); break;
	case 0x4a: lsr_acc_full(); break;
	case 0x4b: asr_imm_full(); break;
	case 0x4c: jmp_adr_full(); break;
	case 0x4d: eor_aba_full(); break;
	case 0x4e: lsr_aba_full(); break;
	case 0x4f: sre_aba_full(); break;
	case 0x50: bvc_rel_full(); break;
	case 0x51: eor_idy_full(); break;
	case 0x52: kil_non_full(); break;
	case 0x53: sre_idy_full(); break;
	case 0x54: nop_zpx_full(); break;
	case 0x55: eor_zpx_full(); break;
	case 0x56: lsr_zpx_full(); break;
	case 0x57: sre_zpx_full(); break;
	case 0x58: cli_imp_full(); break;
	case 0x59: eor_aby_full(); break;
	case 0x5a: nop_imp_full(); break;
	case 0x5b: sre_aby_full(); break;
	case 0x5c: nop_abx_full(); break;
	case 0x5d: eor_abx_full(); break;
	case 0x5e: lsr_abx_full(); break;
	case 0x5f: sre_abx_full(); break;
	case 0x60: rts_imp_full(); break;
	case 0x61: adc_idx_full(); break;
	case 0x62: kil_non_full(); break;
	case 0x63: rra_idx_full(); break;
	case 0x64: nop_zpg_full(); break;
	case 0x65: adc_zpg_full(); break;
	case 0x66: ror_zpg_full(); break;
	case 0x67: rra_zpg_full(); break;
	case 0x68: pla_imp_full(); break;
	case 0x69: adc_imm_full(); break;
	case 0x6a: ror_acc_full(); break;
	case 0x6b: arr_imm_full(); break;
	case 0x6c: jmp_ind_full(); break;
	case 0x6d: adc_aba_full(); break;
	case 0x6e: ror_aba_full(); break;
	case 0x6f: rra_aba_full(); break;
	case 0x70: bvs_rel_full(); break;
	case 0x71: adc_idy_full(); break;
	case 0x72: kil_non_full(); break;
	case 0x73: rra_idy_full(); break;
	case 0x74: nop_zpx_full(); break;
	case 0x75: adc_zpx_full(); break;
	case 0x76: ror_zpx_full(); break;
	case 0x77: rra_zpx_full(); break;
	case 0x78: sei_imp_full(); break;
	case 0x79: adc_aby_full(); break;
	case 0x7a: nop_imp_full(); break;
	case 0x7b: rra_aby_full(); break;
	case 0x7c: nop_abx_full(); break;
	case 0x7d: adc_abx_full(); break;
	case 0x7e: ror_abx_full(); break;
	case 0x7f: rra_abx_full(); break;
	case 0x80: nop_imm_full(); break;
	case 0x81: sta_idx_full(); break;
	case 0x82: nop_imm_full(); break;
	case 0x83: sax_idx_full(); break;
	case 0x84: sty_zpg_full(); break;
	case 0x85: sta_zpg_full(); break;
	case 0x86: stx_zpg_full(); break;
	case 0x87: sax_zpg_full(); break;
	case 0x88: dey_imp_full(); break;
	case 0x89: nop_imm_full(); break;
	case 0x8a: txa_imp_full(); break;
	case 0x8b: ane_imm_full(); break;
	case 0x8c: sty_aba_full(); break;
	case 0x8d: sta_aba_full(); break;
	case 0x8e: stx_aba_full(); break;
	case 0x8f: sax_aba_full(); break;
	case 0x90: bcc_rel_full(); break;
	case 0x91: sta_idy_full(); break;
	case 0x92: kil_non_full(); break;
	case 0x93: sha_idy_full(); break;
	case 0x94: sty_zpx_full(); break;
	case 0x95: sta_zpx_full(); break;
	case 0x96: stx_zpy_full(); break;
	case 0x97: sax_zpy_full(); break;
	case 0x98: tya_imp_full(); break;
	case 0x99: sta_aby_full(); break;
	case 0x9a: txs_imp_full(); break;
	case 0x9b: shs_aby_full(); break;
	case 0x9c: shy_abx_full(); break;
	case 0x9d: sta_abx_full(); break;
	case 0x9e: shx_aby_full(); break;
	case 0x9f: sha_aby_full(); break;
	case 0xa0: ldy_imm_full(); break;
	case 0xa1: lda_idx_full(); break;
	case 0xa2: ldx_imm_full(); break;
	case 0xa3: lax_idx_full(); break;
	case 0xa4: ldy_zpg_full(); break;
	case 0xa5: lda_zpg_full(); break;
	case 0xa6: ldx_zpg_full(); break;
	case 0xa7: lax_zpg_full(); break;
	case 0xa8: tay_imp_full(); break;
	case 0xa9: lda_imm_full(); break;
	case 0xaa: tax_imp_full(); break;
	case 0xab: lxa_imm_full(); break;
	case 0xac: ldy_aba_full(); break;
	case 0xad: lda_aba_full(); break;
	case 0xae: ldx_aba_full(); break;
	case 0xaf: lax_aba_full(); break;
	case 0xb0: bcs_rel_full(); break;
	case 0xb1: lda_idy_full(); break;
	case 0xb2: kil_non_full(); break;
	case 0xb3: lax_idy_full(); break;
	case 0xb4: ldy_zpx_full(); break;
	case 0xb5: lda_zpx_full(); break;
	case 0xb6: ldx_zpy_full(); break;
	case 0xb7: lax_zpy_full(); break;
	case 0xb8: clv_imp_full(); break;
	case 0xb9: lda_aby_full(); break;
	case 0xba: tsx_imp_full(); break;
	case 0xbb: las_aby_full(); break;
	case 0xbc: ldy_abx_full(); break;
	case 0xbd: lda_abx_full(); break;
	case 0xbe: ldx_aby_full(); break;
	case 0xbf: lax_aby_full(); break;
	case 0xc0: cpy_imm_full(); break;
	case 0xc1: cmp_idx_full(); break;
	case 0xc2: nop_imm_full(); break;
	case 0xc3: dcp_idx_full(); break;
	case 0xc4: cpy_zpg_full(); break;
	case 0xc5: cmp_zpg_full(); break;
	case 0xc6: dec_zpg_full(); break;
	case 0xc7: dcp_zpg_full(); break;
	case 0xc8: iny_imp_full(); break;
	case 0xc9: cmp_imm_full(); break;
	case 0xca: dex_imp_full(); break;
	case 0xcb: sbx_imm_full(); break;
	case 0xcc: cpy_aba_full(); break;
	case 0xcd: cmp_aba_full(); break;
	case 0xce: dec_aba_full(); break;
	case 0xcf: dcp_aba_full(); break;
	case 0xd0: bne_rel_full(); break;
	case 0xd1: cmp_idy_full(); break;
	case 0xd2: kil_non_full(); break;
	case 0xd3: dcp_idy_full(); break;
	case 0xd4: nop_zpx_full(); break;
	case 0xd5: cmp_zpx_full(); break;
	case 0xd6: dec_zpx_full(); break;
	case 0xd7: dcp_zpx_full(); break;
	case 0xd8: cld_imp_full(); break;
	case 0xd9: cmp_aby_full(); break;
	case 0xda: nop_imp_full(); break;
	case 0xdb: dcp_aby_full(); break;
	case 0xdc: nop_abx_full(); break;
	case 0xdd: cmp_abx_full(); break;
	case 0xde: dec_abx_full(); break;
	case 0xdf: dcp_abx_full(); break;
	case 0xe0: cpx_imm_full(); break;
	case 0xe1: sbc_idx_full(); break;
	case 0xe2: nop_imm_full(); break;
	case 0xe3: isb_idx_full(); break;
	case 0xe4: cpx_zpg_full(); break;
	case 0xe5: sbc_zpg_full(); break;
	case 0xe6: inc_zpg_full(); break;
	case 0xe7: isb_zpg_full(); break;
	case 0xe8: inx_imp_full(); break;
	case 0xe9: sbc_imm_full(); break;
	case 0xea: nop_imp_full(); break;
	case 0xeb: sbc_imm_full(); break;
	case 0xec: cpx_aba_full(); break;
	case 0xed: sbc_aba_full(); break;
	case 0xee: inc_aba_full(); break;
	case 0xef: isb_aba_full(); break;
	case 0xf0: beq_rel_full(); break;
	case 0xf1: sbc_idy_full(); break;
	case 0xf2: kil_non_full(); break;
	case 0xf3: isb_idy_full(); break;
	case 0xf4: nop_zpx_full(); break;
	case 0xf5: sbc_zpx_full(); break;
	case 0xf6: inc_zpx_full(); break;
	case 0xf7: isb_zpx_full(); break;
	case 0xf8: sed_imp_full(); break;
	case 0xf9: sbc_aby_full(); break;
	case 0xfa: nop_imp_full(); break;
	case 0xfb: isb_aby_full(); break;
	case 0xfc: nop_abx_full(); break;
	case 0xfd: sbc_abx_full(); break;
	case 0xfe: inc_abx_full(); break;
	case 0xff: isb_abx_full(); break;
	case STATE_RESET: reset_full(); break;
	}
}

void m6502_device::do_exec_partial()
{
	switch(inst_state) {

	case 0x00: brk_imp_partial(); break;
	case 0x01: ora_idx_partial(); break;
	case 0x02: kil_non_partial(); break;
	case 0x03: slo_idx_partial(); break;
	case 0x04: nop_zpg_partial(); break;
	case 0x05: ora_zpg_partial(); break;
	case 0x06: asl_zpg_partial(); break;
	case 0x07: slo_zpg_partial(); break;
	case 0x08: php_imp_partial(); break;
	case 0x09: ora_imm_partial(); break;
	case 0x0a: asl_acc_partial(); break;
	case 0x0b: anc_imm_partial(); break;
	case 0x0c: nop_aba_partial(); break;
	case 0x0d: ora_aba_partial(); break;
	case 0x0e: asl_aba_partial(); break;
	case 0x0f: slo_aba_partial(); break;
	case 0x10: bpl_rel_partial(); break;
	case 0x11: ora_idy_partial(); break;
	case 0x12: kil_non_partial(); break;
	case 0x13: slo_idy_partial(); break;
	case 0x14: nop_zpx_partial(); break;
	case 0x15: ora_zpx_partial(); break;
	case 0x16: asl_zpx_partial(); break;
	case 0x17: slo_zpx_partial(); break;
	case 0x18: clc_imp_partial(); break;
	case 0x19: ora_aby_partial(); break;
	case 0x1a: nop_imp_partial(); break;
	case 0x1b: slo_aby_partial(); break;
	case 0x1c: nop_abx_partial(); break;
	case 0x1d: ora_abx_partial(); break;
	case 0x1e: asl_abx_partial(); break;
	case 0x1f: slo_abx_partial(); break;
	case 0x20: jsr_adr_partial(); break;
	case 0x21: and_idx_partial(); break;
	case 0x22: kil_non_partial(); break;
	case 0x23: rla_idx_partial(); break;
	case 0x24: bit_zpg_partial(); break;
	case 0x25: and_zpg_partial(); break;
	case 0x26: rol_zpg_partial(); break;
	case 0x27: rla_zpg_partial(); break;
	case 0x28: plp_imp_partial(); break;
	case 0x29: and_imm_partial(); break;
	case 0x2a: rol_acc_partial(); break;
	case 0x2b: anc_imm_partial(); break;
	case 0x2c: bit_aba_partial(); break;
	case 0x2d: and_aba_partial(); break;
	case 0x2e: rol_aba_partial(); break;
	case 0x2f: rla_aba_partial(); break;
	case 0x30: bmi_rel_partial(); break;
	case 0x31: and_idy_partial(); break;
	case 0x32: kil_non_partial(); break;
	case 0x33: rla_idy_partial(); break;
	case 0x34: nop_zpx_partial(); break;
	case 0x35: and_zpx_partial(); break;
	case 0x36: rol_zpx_partial(); break;
	case 0x37: rla_zpx_partial(); break;
	case 0x38: sec_imp_partial(); break;
	case 0x39: and_aby_partial(); break;
	case 0x3a: nop_imp_partial(); break;
	case 0x3b: rla_aby_partial(); break;
	case 0x3c: nop_abx_partial(); break;
	case 0x3d: and_abx_partial(); break;
	case 0x3e: rol_abx_partial(); break;
	case 0x3f: rla_abx_partial(); break;
	case 0x40: rti_imp_partial(); break;
	case 0x41: eor_idx_partial(); break;
	case 0x42: kil_non_partial(); break;
	case 0x43: sre_idx_partial(); break;
	case 0x44: nop_zpg_partial(); break;
	case 0x45: eor_zpg_partial(); break;
	case 0x46: lsr_zpg_partial(); break;
	case 0x47: sre_zpg_partial(); break;
	case 0x48: pha_imp_partial(); break;
	case 0x49: eor_imm_partial(); break;
	case 0x4a: lsr_acc_partial(); break;
	case 0x4b: asr_imm_partial(); break;
	case 0x4c: jmp_adr_partial(); break;
	case 0x4d: eor_aba_partial(); break;
	case 0x4e: lsr_aba_partial(); break;
	case 0x4f: sre_aba_partial(); break;
	case 0x50: bvc_rel_partial(); break;
	case 0x51: eor_idy_partial(); break;
	case 0x52: kil_non_partial(); break;
	case 0x53: sre_idy_partial(); break;
	case 0x54: nop_zpx_partial(); break;
	case 0x55: eor_zpx_partial(); break;
	case 0x56: lsr_zpx_partial(); break;
	case 0x57: sre_zpx_partial(); break;
	case 0x58: cli_imp_partial(); break;
	case 0x59: eor_aby_partial(); break;
	case 0x5a: nop_imp_partial(); break;
	case 0x5b: sre_aby_partial(); break;
	case 0x5c: nop_abx_partial(); break;
	case 0x5d: eor_abx_partial(); break;
	case 0x5e: lsr_abx_partial(); break;
	case 0x5f: sre_abx_partial(); break;
	case 0x60: rts_imp_partial(); break;
	case 0x61: adc_idx_partial(); break;
	case 0x62: kil_non_partial(); break;
	case 0x63: rra_idx_partial(); break;
	case 0x64: nop_zpg_partial(); break;
	case 0x65: adc_zpg_partial(); break;
	case 0x66: ror_zpg_partial(); break;
	case 0x67: rra_zpg_partial(); break;
	case 0x68: pla_imp_partial(); break;
	case 0x69: adc_imm_partial(); break;
	case 0x6a: ror_acc_partial(); break;
	case 0x6b: arr_imm_partial(); break;
	case 0x6c: jmp_ind_partial(); break;
	case 0x6d: adc_aba_partial(); break;
	case 0x6e: ror_aba_partial(); break;
	case 0x6f: rra_aba_partial(); break;
	case 0x70: bvs_rel_partial(); break;
	case 0x71: adc_idy_partial(); break;
	case 0x72: kil_non_partial(); break;
	case 0x73: rra_idy_partial(); break;
	case 0x74: nop_zpx_partial(); break;
	case 0x75: adc_zpx_partial(); break;
	case 0x76: ror_zpx_partial(); break;
	case 0x77: rra_zpx_partial(); break;
	case 0x78: sei_imp_partial(); break;
	case 0x79: adc_aby_partial(); break;
	case 0x7a: nop_imp_partial(); break;
	case 0x7b: rra_aby_partial(); break;
	case 0x7c: nop_abx_partial(); break;
	case 0x7d: adc_abx_partial(); break;
	case 0x7e: ror_abx_partial(); break;
	case 0x7f: rra_abx_partial(); break;
	case 0x80: nop_imm_partial(); break;
	case 0x81: sta_idx_partial(); break;
	case 0x82: nop_imm_partial(); break;
	case 0x83: sax_idx_partial(); break;
	case 0x84: sty_zpg_partial(); break;
	case 0x85: sta_zpg_partial(); break;
	case 0x86: stx_zpg_partial(); break;
	case 0x87: sax_zpg_partial(); break;
	case 0x88: dey_imp_partial(); break;
	case 0x89: nop_imm_partial(); break;
	case 0x8a: txa_imp_partial(); break;
	case 0x8b: ane_imm_partial(); break;
	case 0x8c: sty_aba_partial(); break;
	case 0x8d: sta_aba_partial(); break;
	case 0x8e: stx_aba_partial(); break;
	case 0x8f: sax_aba_partial(); break;
	case 0x90: bcc_rel_partial(); break;
	case 0x91: sta_idy_partial(); break;
	case 0x92: kil_non_partial(); break;
	case 0x93: sha_idy_partial(); break;
	case 0x94: sty_zpx_partial(); break;
	case 0x95: sta_zpx_partial(); break;
	case 0x96: stx_zpy_partial(); break;
	case 0x97: sax_zpy_partial(); break;
	case 0x98: tya_imp_partial(); break;
	case 0x99: sta_aby_partial(); break;
	case 0x9a: txs_imp_partial(); break;
	case 0x9b: shs_aby_partial(); break;
	case 0x9c: shy_abx_partial(); break;
	case 0x9d: sta_abx_partial(); break;
	case 0x9e: shx_aby_partial(); break;
	case 0x9f: sha_aby_partial(); break;
	case 0xa0: ldy_imm_partial(); break;
	case 0xa1: lda_idx_partial(); break;
	case 0xa2: ldx_imm_partial(); break;
	case 0xa3: lax_idx_partial(); break;
	case 0xa4: ldy_zpg_partial(); break;
	case 0xa5: lda_zpg_partial(); break;
	case 0xa6: ldx_zpg_partial(); break;
	case 0xa7: lax_zpg_partial(); break;
	case 0xa8: tay_imp_partial(); break;
	case 0xa9: lda_imm_partial(); break;
	case 0xaa: tax_imp_partial(); break;
	case 0xab: lxa_imm_partial(); break;
	case 0xac: ldy_aba_partial(); break;
	case 0xad: lda_aba_partial(); break;
	case 0xae: ldx_aba_partial(); break;
	case 0xaf: lax_aba_partial(); break;
	case 0xb0: bcs_rel_partial(); break;
	case 0xb1: lda_idy_partial(); break;
	case 0xb2: kil_non_partial(); break;
	case 0xb3: lax_idy_partial(); break;
	case 0xb4: ldy_zpx_partial(); break;
	case 0xb5: lda_zpx_partial(); break;
	case 0xb6: ldx_zpy_partial(); break;
	case 0xb7: lax_zpy_partial(); break;
	case 0xb8: clv_imp_partial(); break;
	case 0xb9: lda_aby_partial(); break;
	case 0xba: tsx_imp_partial(); break;
	case 0xbb: las_aby_partial(); break;
	case 0xbc: ldy_abx_partial(); break;
	case 0xbd: lda_abx_partial(); break;
	case 0xbe: ldx_aby_partial(); break;
	case 0xbf: lax_aby_partial(); break;
	case 0xc0: cpy_imm_partial(); break;
	case 0xc1: cmp_idx_partial(); break;
	case 0xc2: nop_imm_partial(); break;
	case 0xc3: dcp_idx_partial(); break;
	case 0xc4: cpy_zpg_partial(); break;
	case 0xc5: cmp_zpg_partial(); break;
	case 0xc6: dec_zpg_partial(); break;
	case 0xc7: dcp_zpg_partial(); break;
	case 0xc8: iny_imp_partial(); break;
	case 0xc9: cmp_imm_partial(); break;
	case 0xca: dex_imp_partial(); break;
	case 0xcb: sbx_imm_partial(); break;
	case 0xcc: cpy_aba_partial(); break;
	case 0xcd: cmp_aba_partial(); break;
	case 0xce: dec_aba_partial(); break;
	case 0xcf: dcp_aba_partial(); break;
	case 0xd0: bne_rel_partial(); break;
	case 0xd1: cmp_idy_partial(); break;
	case 0xd2: kil_non_partial(); break;
	case 0xd3: dcp_idy_partial(); break;
	case 0xd4: nop_zpx_partial(); break;
	case 0xd5: cmp_zpx_partial(); break;
	case 0xd6: dec_zpx_partial(); break;
	case 0xd7: dcp_zpx_partial(); break;
	case 0xd8: cld_imp_partial(); break;
	case 0xd9: cmp_aby_partial(); break;
	case 0xda: nop_imp_partial(); break;
	case 0xdb: dcp_aby_partial(); break;
	case 0xdc: nop_abx_partial(); break;
	case 0xdd: cmp_abx_partial(); break;
	case 0xde: dec_abx_partial(); break;
	case 0xdf: dcp_abx_partial(); break;
	case 0xe0: cpx_imm_partial(); break;
	case 0xe1: sbc_idx_partial(); break;
	case 0xe2: nop_imm_partial(); break;
	case 0xe3: isb_idx_partial(); break;
	case 0xe4: cpx_zpg_partial(); break;
	case 0xe5: sbc_zpg_partial(); break;
	case 0xe6: inc_zpg_partial(); break;
	case 0xe7: isb_zpg_partial(); break;
	case 0xe8: inx_imp_partial(); break;
	case 0xe9: sbc_imm_partial(); break;
	case 0xea: nop_imp_partial(); break;
	case 0xeb: sbc_imm_partial(); break;
	case 0xec: cpx_aba_partial(); break;
	case 0xed: sbc_aba_partial(); break;
	case 0xee: inc_aba_partial(); break;
	case 0xef: isb_aba_partial(); break;
	case 0xf0: beq_rel_partial(); break;
	case 0xf1: sbc_idy_partial(); break;
	case 0xf2: kil_non_partial(); break;
	case 0xf3: isb_idy_partial(); break;
	case 0xf4: nop_zpx_partial(); break;
	case 0xf5: sbc_zpx_partial(); break;
	case 0xf6: inc_zpx_partial(); break;
	case 0xf7: isb_zpx_partial(); break;
	case 0xf8: sed_imp_partial(); break;
	case 0xf9: sbc_aby_partial(); break;
	case 0xfa: nop_imp_partial(); break;
	case 0xfb: isb_aby_partial(); break;
	case 0xfc: nop_abx_partial(); break;
	case 0xfd: sbc_abx_partial(); break;
	case 0xfe: inc_abx_partial(); break;
	case 0xff: isb_abx_partial(); break;
	case STATE_RESET: reset_partial(); break;
	}
}

