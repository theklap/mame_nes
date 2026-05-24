void w65c02s_device::adc_s_abx_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, X)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	do_adc(TMP2);
	if(P & F_D) {
		read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
		set_nz(A);
	}
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::adc_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	if(P & F_D) {
	[[fallthrough]];
case 9:
		read_pc();
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
		set_nz(A);
	}
	PC++;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::adc_s_aby_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, Y)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	do_adc(TMP2);
	if(P & F_D) {
		read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
		set_nz(A);
	}
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::adc_s_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	if(P & F_D) {
	[[fallthrough]];
case 9:
		read_pc();
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
		set_nz(A);
	}
	PC++;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::adc_s_idx_full()
{
	TMP2 = read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	do_adc(TMP2);
	if(P & F_D) {
		read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
		set_nz(A);
	}
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::adc_s_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
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
	if(P & F_D) {
	[[fallthrough]];
case 11:
		read_pc();
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
		set_nz(A);
	}
	PC++;
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::adc_s_idy_full()
{
	TMP2 = read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	TMP = read(TMP2);
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(page_changing(TMP, Y)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	do_adc(TMP2);
	if(P & F_D) {
		read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
		set_nz(A);
	}
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::adc_s_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
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
	TMP = read(TMP2);
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
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
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
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 7:
		read_pc();
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
	}
	[[fallthrough]];
case 9:
	TMP2 = read(TMP+Y);
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
	if(P & F_D) {
	[[fallthrough]];
case 11:
		read_pc();
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
		set_nz(A);
	}
	PC++;
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::adc_s_zpx_full()
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
	read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	do_adc(TMP2);
	if(P & F_D) {
		read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
		set_nz(A);
	}
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::adc_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	[[fallthrough]];
case 5:
	TMP2 = read(uint8_t(TMP+X));
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
	do_adc(TMP2);
	if(P & F_D) {
	[[fallthrough]];
case 7:
		read_pc();
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
		set_nz(A);
	}
	PC++;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::and_s_abx_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, X)) {
		read_pc();
	icount--;
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
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::and_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::and_s_aby_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, Y)) {
		read_pc();
	icount--;
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
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::and_s_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::and_s_idx_full()
{
	TMP2 = read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::and_s_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
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
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
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
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
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
	TMP2 = read(TMP);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::and_s_idy_full()
{
	TMP2 = read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	TMP = read(TMP2);
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(page_changing(TMP, Y)) {
		read_pc();
	icount--;
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
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::and_s_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
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
	TMP = read(TMP2);
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
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
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
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 7:
		read_pc();
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
	}
	[[fallthrough]];
case 9:
	TMP2 = read(TMP+Y);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::and_s_zpx_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(uint8_t(TMP+X));
	icount--;
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::and_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(uint8_t(TMP+X));
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
	A &= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::asl_s_abx_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, X)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	read(TMP+1);
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
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::asl_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	read(TMP+1);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::asl_s_zpg_full()
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
	TMP2 = read(TMP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::asl_s_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
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
	[[fallthrough]];
case 5:
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::asl_s_zpx_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::asl_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
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
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::bit_s_abx_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, X)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	do_bit(TMP2);
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::bit_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	do_bit(TMP2);
	PC++;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::bit_s_zpx_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(uint8_t(TMP+X));
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	do_bit(TMP2);
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::bit_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(uint8_t(TMP+X));
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
	do_bit(TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::brk_s_imp_full()
{
	if(irq_taken || nmi_pending) {
		read_pc();
	icount--;
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
	icount--;
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
	write(SP, PC);
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
	write(SP, irq_taken || nmi_pending ? P & ~F_B : P);
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
	if(irq_taken && nmi_pending) {
		standard_irq_callback(NMI_LINE, PC);
		PC = read_vector(0xfffa);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
		PC = set_h(PC, read_vector(0xfffb));
	icount--;
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
		PC = read_vector(0xfffe);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 15;
		} else
			inst_substate = 16;
		return;
	}
		PC = set_h(PC, read_vector(0xffff));
	icount--;
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
	P = (P | F_I) & ~F_D; // Do *not* move after the prefetch
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::brk_s_imp_partial()
{
	switch(inst_substate) {
case 0:
	if(irq_taken || nmi_pending) {
	[[fallthrough]];
case 1:
		read_pc();
	icount--;
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
	write(SP, PC);
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
	write(SP, irq_taken || nmi_pending ? P & ~F_B : P);
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
	if(irq_taken && nmi_pending) {
		standard_irq_callback(NMI_LINE, PC);
	[[fallthrough]];
case 11:
		PC = read_vector(0xfffa);
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
	[[fallthrough]];
case 13:
		PC = set_h(PC, read_vector(0xfffb));
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
		nmi_pending = false;
	} else {
		if(irq_taken)
			standard_irq_callback(IRQ_LINE, PC);
	[[fallthrough]];
case 15:
		PC = read_vector(0xfffe);
	icount--;
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
		PC = set_h(PC, read_vector(0xffff));
	icount--;
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
	P = (P | F_I) & ~F_D; // Do *not* move after the prefetch
	prefetch_start();
	[[fallthrough]];
case 19:
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::cmp_s_abx_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, X)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	do_cmp(A, TMP2);
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::cmp_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::cmp_s_aby_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, Y)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	do_cmp(A, TMP2);
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::cmp_s_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::cmp_s_idx_full()
{
	TMP2 = read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::cmp_s_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
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
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
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
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
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
	TMP2 = read(TMP);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::cmp_s_idy_full()
{
	TMP2 = read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	TMP = read(TMP2);
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(page_changing(TMP, Y)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	do_cmp(A, TMP2);
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::cmp_s_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
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
	TMP = read(TMP2);
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
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
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
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 7:
		read_pc();
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
	}
	[[fallthrough]];
case 9:
	TMP2 = read(TMP+Y);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::cmp_s_zpx_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP = read(uint8_t(TMP+X));
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	do_cmp(A, TMP);
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::cmp_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	[[fallthrough]];
case 5:
	TMP = read(uint8_t(TMP+X));
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
	do_cmp(A, TMP);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::dec_s_abx_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, X)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	read(TMP+1);
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
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::dec_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	read(TMP+1);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::dec_s_zpg_full()
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
	TMP2 = read(TMP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::dec_s_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
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
	[[fallthrough]];
case 5:
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::dec_s_zpx_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::dec_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
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
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::eor_s_abx_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, X)) {
		read_pc();
	icount--;
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
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::eor_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::eor_s_aby_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, Y)) {
		read_pc();
	icount--;
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
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::eor_s_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::eor_s_idx_full()
{
	TMP2 = read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::eor_s_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
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
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
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
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
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
	TMP2 = read(TMP);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::eor_s_idy_full()
{
	TMP2 = read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	TMP = read(TMP2);
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(page_changing(TMP, Y)) {
		read_pc();
	icount--;
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
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::eor_s_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
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
	TMP = read(TMP2);
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
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
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
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 7:
		read_pc();
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
	}
	[[fallthrough]];
case 9:
	TMP2 = read(TMP+Y);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::eor_s_zpx_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(uint8_t(TMP+X));
	icount--;
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::eor_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(uint8_t(TMP+X));
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
	A ^= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::inc_s_abx_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, X)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	read(TMP+1);
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
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::inc_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	read(TMP+1);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::inc_s_zpg_full()
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
	TMP2 = read(TMP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::inc_s_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
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
	[[fallthrough]];
case 5:
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::inc_s_zpx_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::inc_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
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
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::jmp_s_iax_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	TMP += X;
	PC = read(TMP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	PC = set_h(PC, read(TMP+1));
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::jmp_s_iax_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	[[fallthrough]];
case 5:
	read_pc();
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
	TMP += X;
	[[fallthrough]];
case 7:
	PC = read(TMP);
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
	PC = set_h(PC, read(TMP+1));
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::jmp_s_ind_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	PC = read(TMP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	PC = set_h(PC, read(TMP+1));
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::jmp_s_ind_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	[[fallthrough]];
case 5:
	read_pc();
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
	PC = read(TMP);
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
	PC = set_h(PC, read(TMP+1));
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::lda_s_abx_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, X)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	set_nz(A);
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::lda_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	[[fallthrough]];
case 7:
	A = read(TMP + X);
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
	set_nz(A);
	PC++;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::lda_s_aby_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, Y)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	set_nz(A);
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::lda_s_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	[[fallthrough]];
case 7:
	A = read(TMP + Y);
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
	set_nz(A);
	PC++;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::lda_s_idx_full()
{
	TMP2 = read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::lda_s_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
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
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
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
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
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
	A = read(TMP);
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
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::lda_s_idy_full()
{
	TMP2 = read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	TMP = read(TMP2);
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(page_changing(TMP, Y)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	set_nz(A);
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::lda_s_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
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
	TMP = read(TMP2);
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
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
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
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 7:
		read_pc();
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
	}
	[[fallthrough]];
case 9:
	A = read(TMP+Y);
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
	set_nz(A);
	PC++;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::lda_s_zpx_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	A = read(uint8_t(TMP+X));
	icount--;
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::lda_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	[[fallthrough]];
case 5:
	A = read(uint8_t(TMP+X));
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
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::ldx_s_aby_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, Y)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	set_nz(X);
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::ldx_s_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	[[fallthrough]];
case 7:
	X = read(TMP + Y);
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
	set_nz(X);
	PC++;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::ldx_s_zpy_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	X = read(uint8_t(TMP+Y));
	icount--;
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::ldx_s_zpy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	[[fallthrough]];
case 5:
	X = read(uint8_t(TMP+Y));
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
	set_nz(X);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::ldy_s_abx_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, X)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	set_nz(Y);
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::ldy_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += X;
	[[fallthrough]];
case 7:
	Y = read(TMP);
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
	set_nz(Y);
	PC++;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::ldy_s_zpx_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	Y = read(uint8_t(TMP+X));
	icount--;
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::ldy_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	[[fallthrough]];
case 5:
	Y = read(uint8_t(TMP+X));
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
	set_nz(Y);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::lsr_s_abx_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, X)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	read((TMP+1) & 0xff);
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
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::lsr_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	read((TMP+1) & 0xff);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::lsr_s_zpg_full()
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
	TMP2 = read(TMP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::lsr_s_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
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
	[[fallthrough]];
case 5:
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::lsr_s_zpx_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::lsr_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
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
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::nop_s_abx_full()
{
	read_pc();
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::nop_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
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
	read_pc();
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
	[[fallthrough]];
case 5:
	read_pc();
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::nop_s_zpx_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	read(uint8_t(TMP+X));
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::nop_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	[[fallthrough]];
case 5:
	read(uint8_t(TMP+X));
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::ora_s_abx_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, X)) {
		read_pc();
	icount--;
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
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::ora_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::ora_s_aby_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, Y)) {
		read_pc();
	icount--;
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
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::ora_s_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::ora_s_idx_full()
{
	TMP2 = read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::ora_s_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
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
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
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
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
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
	TMP2 = read(TMP);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::ora_s_idy_full()
{
	TMP2 = read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	TMP = read(TMP2);
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(page_changing(TMP, Y)) {
		read_pc();
	icount--;
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
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::ora_s_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
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
	TMP = read(TMP2);
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
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
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
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 7:
		read_pc();
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
	}
	[[fallthrough]];
case 9:
	TMP2 = read(TMP+Y);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::ora_s_zpx_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 = read(uint8_t(TMP+X));
	icount--;
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::ora_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	[[fallthrough]];
case 5:
	TMP2 = read(uint8_t(TMP+X));
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
	A |= TMP2;
	set_nz(A);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::pla_s_imp_full()
{
	read_pc();
	icount--;
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::pla_s_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::plp_s_imp_full()
{
	read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	inc_SP();
	TMP = read(SP) | (F_B|F_E);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::plp_s_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
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
	TMP = read(SP) | (F_B|F_E);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	P = TMP; // Do *not* move it before the prefetch
	break;
}
	inst_substate = 0;
}

void w65c02s_device::plx_s_imp_full()
{
	read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	inc_SP();
	X = read(SP);
	icount--;
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::plx_s_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
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
	X = read(SP);
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
	set_nz(X);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::ply_s_imp_full()
{
	read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	inc_SP();
	Y = read(SP);
	icount--;
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::ply_s_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
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
	Y = read(SP);
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
	set_nz(Y);
	prefetch_start();
	[[fallthrough]];
case 7:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::rol_s_abx_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, X)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	read(TMP+1);
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
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::rol_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	read(TMP+1);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::rol_s_zpg_full()
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
	TMP2 = read(TMP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::rol_s_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
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
	[[fallthrough]];
case 5:
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::rol_s_zpx_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::rol_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
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
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::ror_s_abx_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, X)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	read(TMP+1);
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
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::ror_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	read(TMP+1);
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
	PC++;
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::ror_s_zpg_full()
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
	TMP2 = read(TMP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::ror_s_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
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
	[[fallthrough]];
case 5:
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::ror_s_zpx_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP = uint8_t(TMP+X);
	TMP2 = read(TMP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::ror_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	TMP = uint8_t(TMP+X);
	[[fallthrough]];
case 5:
	TMP2 = read(TMP);
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
	read((TMP+1) & 0xff);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::rti_s_imp_full()
{
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	inc_SP();
	P = read(SP) | (F_B|F_E);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	inc_SP();
	TMP = read(SP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	inc_SP();
	TMP = set_h(TMP, read(SP));
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	end_interrupt();
	PC = TMP;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::rti_s_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
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
	inc_SP();
	[[fallthrough]];
case 3:
	P = read(SP) | (F_B|F_E);
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
	inc_SP();
	[[fallthrough]];
case 5:
	TMP = read(SP);
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
	inc_SP();
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read(SP));
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
	read_pc();
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
	end_interrupt();
	PC = TMP;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::rts_s_imp_full()
{
	read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	inc_SP();
	TMP = read(SP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	inc_SP();
	TMP = set_h(TMP, read(SP));
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	PC = TMP + 1;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::rts_s_imp_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	icount--;
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
	TMP = read(SP);
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
	inc_SP();
	[[fallthrough]];
case 7:
	TMP = set_h(TMP, read(SP));
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
	read_pc();
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
	PC = TMP + 1;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::sbc_s_abx_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, X)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	do_sbc(TMP2);
	if(P & F_D) {
		read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
		set_nz(A);
	}
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::sbc_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, X)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += X;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	if(P & F_D) {
	[[fallthrough]];
case 9:
		read_pc();
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
		set_nz(A);
	}
	PC++;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::sbc_s_aby_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	if(page_changing(TMP, Y)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	do_sbc(TMP2);
	if(P & F_D) {
		read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
		set_nz(A);
	}
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::sbc_s_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 5:
		read_pc();
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
	}
	TMP += Y;
	[[fallthrough]];
case 7:
	TMP2 = read(TMP);
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
	if(P & F_D) {
	[[fallthrough]];
case 9:
		read_pc();
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
		set_nz(A);
	}
	PC++;
	prefetch_start();
	[[fallthrough]];
case 11:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::sbc_s_idx_full()
{
	TMP2 = read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	do_sbc(TMP2);
	if(P & F_D) {
		read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
		set_nz(A);
	}
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::sbc_s_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
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
	if(P & F_D) {
	[[fallthrough]];
case 11:
		read_pc();
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
		set_nz(A);
	}
	PC++;
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::sbc_s_idy_full()
{
	TMP2 = read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	TMP = read(TMP2);
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(page_changing(TMP, Y)) {
		read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	do_sbc(TMP2);
	if(P & F_D) {
		read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
		set_nz(A);
	}
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::sbc_s_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
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
	TMP = read(TMP2);
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
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
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
	if(page_changing(TMP, Y)) {
	[[fallthrough]];
case 7:
		read_pc();
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
	}
	[[fallthrough]];
case 9:
	TMP2 = read(TMP+Y);
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
	if(P & F_D) {
	[[fallthrough]];
case 11:
		read_pc();
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
		set_nz(A);
	}
	PC++;
	prefetch_start();
	[[fallthrough]];
case 13:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::sbc_s_zpx_full()
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
	read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	do_sbc(TMP2);
	if(P & F_D) {
		read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
		set_nz(A);
	}
	PC++;
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::sbc_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	[[fallthrough]];
case 5:
	TMP2 = read(uint8_t(TMP+X));
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
	do_sbc(TMP2);
	if(P & F_D) {
	[[fallthrough]];
case 7:
		read_pc();
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
		set_nz(A);
	}
	PC++;
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::sta_s_abx_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	PC++;
	write(TMP+X, A);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::sta_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	[[fallthrough]];
case 5:
	read_pc();
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
	PC++;
	[[fallthrough]];
case 7:
	write(TMP+X, A);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::sta_s_aby_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	PC++;
	write(TMP+Y, A);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::sta_s_aby_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	[[fallthrough]];
case 5:
	read_pc();
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
	PC++;
	[[fallthrough]];
case 7:
	write(TMP+Y, A);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::sta_s_idx_full()
{
	TMP2 = read_pc();
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	TMP2 += X;
	TMP = read(TMP2 & 0xff);
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write(TMP, A);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::sta_s_idx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
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
	TMP2 += X;
	[[fallthrough]];
case 5:
	TMP = read(TMP2 & 0xff);
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
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
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
	write(TMP, A);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::sta_s_idy_full()
{
	TMP2 = read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 1;
		} else
			inst_substate = 2;
		return;
	}
	TMP = read(TMP2);
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	PC++;
	write(TMP+Y, A);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::sta_s_idy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	TMP2 = read_pc();
	icount--;
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
	TMP = read(TMP2);
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
	[[fallthrough]];
case 5:
	TMP = set_h(TMP, read((TMP2+1) & 0xff));
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
	read_pc();
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
	PC++;
	[[fallthrough]];
case 9:
	write(TMP+Y, A);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::sta_s_zpx_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	write(uint8_t(TMP+X), A);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::sta_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	[[fallthrough]];
case 5:
	write(uint8_t(TMP+X), A);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::stx_s_zpy_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	write(uint8_t(TMP+Y), X);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::stx_s_zpy_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	[[fallthrough]];
case 5:
	write(uint8_t(TMP+Y), X);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::sty_s_zpx_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	write(uint8_t(TMP+X), Y);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::sty_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	[[fallthrough]];
case 5:
	write(uint8_t(TMP+X), Y);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::stz_s_abx_full()
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
	TMP = set_h(TMP, read_pc());
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	PC++;
	write(TMP+X, 0x00);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::stz_s_abx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP = set_h(TMP, read_pc());
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
	[[fallthrough]];
case 5:
	read_pc();
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
	PC++;
	[[fallthrough]];
case 7:
	write(TMP+X, 0x00);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::stz_s_zpx_full()
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
	read_pc();
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	PC++;
	write(uint8_t(TMP+X), 0x00);
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
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::stz_s_zpx_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
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
	PC++;
	[[fallthrough]];
case 5:
	write(uint8_t(TMP+X), 0x00);
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
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::trb_s_zpg_full()
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
	TMP2 = read(TMP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	read((TMP+1) & 0xff);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(A & TMP2)
		P &= ~F_Z;
	else
		P |= F_Z;
	TMP2 &= ~A;
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
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::trb_s_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
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
	[[fallthrough]];
case 5:
	read((TMP+1) & 0xff);
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
	if(A & TMP2)
		P &= ~F_Z;
	else
		P |= F_Z;
	TMP2 &= ~A;
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
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::tsb_s_zpg_full()
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
	TMP2 = read(TMP);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 3;
		} else
			inst_substate = 4;
		return;
	}
	read((TMP+1) & 0xff);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 5;
		} else
			inst_substate = 6;
		return;
	}
	if(A & TMP2)
		P &= ~F_Z;
	else
		P |= F_Z;
	TMP2 |= A;
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
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::tsb_s_zpg_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
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
	[[fallthrough]];
case 2:;
	PC++;
	[[fallthrough]];
case 3:
	TMP2 = read(TMP);
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
	[[fallthrough]];
case 5:
	read((TMP+1) & 0xff);
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
	if(A & TMP2)
		P &= ~F_Z;
	else
		P |= F_Z;
	TMP2 |= A;
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
	prefetch_start();
	[[fallthrough]];
case 9:
	IR = mintf->read_sync(PC);
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
	prefetch_end();
	break;
}
	inst_substate = 0;
}

void w65c02s_device::reset_s_full()
{
	read_arg(0xffff);
	icount--;
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
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 9;
		} else
			inst_substate = 10;
		return;
	}
	dec_SP();
	P = (P | F_I) & ~F_D;
	PC = read_vector(0xfffc);
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 11;
		} else
			inst_substate = 12;
		return;
	}
	PC = set_h(PC, read_vector(0xfffd));
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 13;
		} else
			inst_substate = 14;
		return;
	}
	prefetch_start();
	IR = mintf->read_sync(PC);
	icount--;
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

void w65c02s_device::reset_s_partial()
{
	switch(inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_arg(0xffff);
	icount--;
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
	P = (P | F_I) & ~F_D;
	[[fallthrough]];
case 11:
	PC = read_vector(0xfffc);
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
	[[fallthrough]];
case 13:
	PC = set_h(PC, read_vector(0xfffd));
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
	prefetch_start();
	[[fallthrough]];
case 15:
	IR = mintf->read_sync(PC);
	icount--;
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



void w65c02s_device::do_exec_full()
{
	switch(inst_state) {

	case 0x00: brk_s_imp_full(); break;
	case 0x01: ora_s_idx_full(); break;
	case 0x02: nop_imm_full(); break;
	case 0x03: nop_c_imp_full(); break;
	case 0x04: tsb_s_zpg_full(); break;
	case 0x05: ora_zpg_full(); break;
	case 0x06: asl_s_zpg_full(); break;
	case 0x07: rmb_bzp_full(); break;
	case 0x08: php_imp_full(); break;
	case 0x09: ora_imm_full(); break;
	case 0x0a: asl_acc_full(); break;
	case 0x0b: nop_c_imp_full(); break;
	case 0x0c: tsb_aba_full(); break;
	case 0x0d: ora_aba_full(); break;
	case 0x0e: asl_c_aba_full(); break;
	case 0x0f: bbr_zpb_full(); break;
	case 0x10: bpl_rel_full(); break;
	case 0x11: ora_s_idy_full(); break;
	case 0x12: ora_zpi_full(); break;
	case 0x13: nop_c_imp_full(); break;
	case 0x14: trb_s_zpg_full(); break;
	case 0x15: ora_s_zpx_full(); break;
	case 0x16: asl_s_zpx_full(); break;
	case 0x17: rmb_bzp_full(); break;
	case 0x18: clc_imp_full(); break;
	case 0x19: ora_s_aby_full(); break;
	case 0x1a: inc_acc_full(); break;
	case 0x1b: nop_c_imp_full(); break;
	case 0x1c: trb_aba_full(); break;
	case 0x1d: ora_s_abx_full(); break;
	case 0x1e: asl_s_abx_full(); break;
	case 0x1f: bbr_zpb_full(); break;
	case 0x20: jsr_adr_full(); break;
	case 0x21: and_s_idx_full(); break;
	case 0x22: nop_imm_full(); break;
	case 0x23: nop_c_imp_full(); break;
	case 0x24: bit_zpg_full(); break;
	case 0x25: and_zpg_full(); break;
	case 0x26: rol_s_zpg_full(); break;
	case 0x27: rmb_bzp_full(); break;
	case 0x28: plp_s_imp_full(); break;
	case 0x29: and_imm_full(); break;
	case 0x2a: rol_acc_full(); break;
	case 0x2b: nop_c_imp_full(); break;
	case 0x2c: bit_aba_full(); break;
	case 0x2d: and_aba_full(); break;
	case 0x2e: rol_c_aba_full(); break;
	case 0x2f: bbr_zpb_full(); break;
	case 0x30: bmi_rel_full(); break;
	case 0x31: and_s_idy_full(); break;
	case 0x32: and_zpi_full(); break;
	case 0x33: nop_c_imp_full(); break;
	case 0x34: bit_s_zpx_full(); break;
	case 0x35: and_s_zpx_full(); break;
	case 0x36: rol_s_zpx_full(); break;
	case 0x37: rmb_bzp_full(); break;
	case 0x38: sec_imp_full(); break;
	case 0x39: and_s_aby_full(); break;
	case 0x3a: dec_acc_full(); break;
	case 0x3b: nop_c_imp_full(); break;
	case 0x3c: bit_s_abx_full(); break;
	case 0x3d: and_s_abx_full(); break;
	case 0x3e: rol_s_abx_full(); break;
	case 0x3f: bbr_zpb_full(); break;
	case 0x40: rti_s_imp_full(); break;
	case 0x41: eor_s_idx_full(); break;
	case 0x42: nop_imm_full(); break;
	case 0x43: nop_c_imp_full(); break;
	case 0x44: nop_zpg_full(); break;
	case 0x45: eor_zpg_full(); break;
	case 0x46: lsr_s_zpg_full(); break;
	case 0x47: rmb_bzp_full(); break;
	case 0x48: pha_imp_full(); break;
	case 0x49: eor_imm_full(); break;
	case 0x4a: lsr_acc_full(); break;
	case 0x4b: nop_c_imp_full(); break;
	case 0x4c: jmp_adr_full(); break;
	case 0x4d: eor_aba_full(); break;
	case 0x4e: lsr_c_aba_full(); break;
	case 0x4f: bbr_zpb_full(); break;
	case 0x50: bvc_rel_full(); break;
	case 0x51: eor_s_idy_full(); break;
	case 0x52: eor_zpi_full(); break;
	case 0x53: nop_c_imp_full(); break;
	case 0x54: nop_s_zpx_full(); break;
	case 0x55: eor_s_zpx_full(); break;
	case 0x56: lsr_s_zpx_full(); break;
	case 0x57: rmb_bzp_full(); break;
	case 0x58: cli_imp_full(); break;
	case 0x59: eor_s_aby_full(); break;
	case 0x5a: phy_imp_full(); break;
	case 0x5b: nop_c_imp_full(); break;
	case 0x5c: nop_c_aba_full(); break;
	case 0x5d: eor_s_abx_full(); break;
	case 0x5e: lsr_s_abx_full(); break;
	case 0x5f: bbr_zpb_full(); break;
	case 0x60: rts_s_imp_full(); break;
	case 0x61: adc_s_idx_full(); break;
	case 0x62: nop_imm_full(); break;
	case 0x63: nop_c_imp_full(); break;
	case 0x64: stz_zpg_full(); break;
	case 0x65: adc_c_zpg_full(); break;
	case 0x66: ror_s_zpg_full(); break;
	case 0x67: rmb_bzp_full(); break;
	case 0x68: pla_s_imp_full(); break;
	case 0x69: adc_c_imm_full(); break;
	case 0x6a: ror_acc_full(); break;
	case 0x6b: nop_c_imp_full(); break;
	case 0x6c: jmp_s_ind_full(); break;
	case 0x6d: adc_c_aba_full(); break;
	case 0x6e: ror_c_aba_full(); break;
	case 0x6f: bbr_zpb_full(); break;
	case 0x70: bvs_rel_full(); break;
	case 0x71: adc_s_idy_full(); break;
	case 0x72: adc_c_zpi_full(); break;
	case 0x73: nop_c_imp_full(); break;
	case 0x74: stz_s_zpx_full(); break;
	case 0x75: adc_s_zpx_full(); break;
	case 0x76: ror_s_zpx_full(); break;
	case 0x77: rmb_bzp_full(); break;
	case 0x78: sei_imp_full(); break;
	case 0x79: adc_s_aby_full(); break;
	case 0x7a: ply_s_imp_full(); break;
	case 0x7b: nop_c_imp_full(); break;
	case 0x7c: jmp_s_iax_full(); break;
	case 0x7d: adc_s_abx_full(); break;
	case 0x7e: ror_s_abx_full(); break;
	case 0x7f: bbr_zpb_full(); break;
	case 0x80: bra_rel_full(); break;
	case 0x81: sta_s_idx_full(); break;
	case 0x82: nop_imm_full(); break;
	case 0x83: nop_c_imp_full(); break;
	case 0x84: sty_zpg_full(); break;
	case 0x85: sta_zpg_full(); break;
	case 0x86: stx_zpg_full(); break;
	case 0x87: smb_bzp_full(); break;
	case 0x88: dey_imp_full(); break;
	case 0x89: bit_imm_full(); break;
	case 0x8a: txa_imp_full(); break;
	case 0x8b: nop_c_imp_full(); break;
	case 0x8c: sty_aba_full(); break;
	case 0x8d: sta_aba_full(); break;
	case 0x8e: stx_aba_full(); break;
	case 0x8f: bbs_zpb_full(); break;
	case 0x90: bcc_rel_full(); break;
	case 0x91: sta_s_idy_full(); break;
	case 0x92: sta_zpi_full(); break;
	case 0x93: nop_c_imp_full(); break;
	case 0x94: sty_s_zpx_full(); break;
	case 0x95: sta_s_zpx_full(); break;
	case 0x96: stx_s_zpy_full(); break;
	case 0x97: smb_bzp_full(); break;
	case 0x98: tya_imp_full(); break;
	case 0x99: sta_s_aby_full(); break;
	case 0x9a: txs_imp_full(); break;
	case 0x9b: nop_c_imp_full(); break;
	case 0x9c: stz_aba_full(); break;
	case 0x9d: sta_s_abx_full(); break;
	case 0x9e: stz_s_abx_full(); break;
	case 0x9f: bbs_zpb_full(); break;
	case 0xa0: ldy_imm_full(); break;
	case 0xa1: lda_s_idx_full(); break;
	case 0xa2: ldx_imm_full(); break;
	case 0xa3: nop_c_imp_full(); break;
	case 0xa4: ldy_zpg_full(); break;
	case 0xa5: lda_zpg_full(); break;
	case 0xa6: ldx_zpg_full(); break;
	case 0xa7: smb_bzp_full(); break;
	case 0xa8: tay_imp_full(); break;
	case 0xa9: lda_imm_full(); break;
	case 0xaa: tax_imp_full(); break;
	case 0xab: nop_c_imp_full(); break;
	case 0xac: ldy_aba_full(); break;
	case 0xad: lda_aba_full(); break;
	case 0xae: ldx_aba_full(); break;
	case 0xaf: bbs_zpb_full(); break;
	case 0xb0: bcs_rel_full(); break;
	case 0xb1: lda_s_idy_full(); break;
	case 0xb2: lda_zpi_full(); break;
	case 0xb3: nop_c_imp_full(); break;
	case 0xb4: ldy_s_zpx_full(); break;
	case 0xb5: lda_s_zpx_full(); break;
	case 0xb6: ldx_s_zpy_full(); break;
	case 0xb7: smb_bzp_full(); break;
	case 0xb8: clv_imp_full(); break;
	case 0xb9: lda_s_aby_full(); break;
	case 0xba: tsx_imp_full(); break;
	case 0xbb: nop_c_imp_full(); break;
	case 0xbc: ldy_s_abx_full(); break;
	case 0xbd: lda_s_abx_full(); break;
	case 0xbe: ldx_s_aby_full(); break;
	case 0xbf: bbs_zpb_full(); break;
	case 0xc0: cpy_imm_full(); break;
	case 0xc1: cmp_s_idx_full(); break;
	case 0xc2: nop_imm_full(); break;
	case 0xc3: nop_c_imp_full(); break;
	case 0xc4: cpy_zpg_full(); break;
	case 0xc5: cmp_zpg_full(); break;
	case 0xc6: dec_s_zpg_full(); break;
	case 0xc7: smb_bzp_full(); break;
	case 0xc8: iny_imp_full(); break;
	case 0xc9: cmp_imm_full(); break;
	case 0xca: dex_imp_full(); break;
	case 0xcb: wai_imp_full(); break;
	case 0xcc: cpy_aba_full(); break;
	case 0xcd: cmp_aba_full(); break;
	case 0xce: dec_c_aba_full(); break;
	case 0xcf: bbs_zpb_full(); break;
	case 0xd0: bne_rel_full(); break;
	case 0xd1: cmp_s_idy_full(); break;
	case 0xd2: cmp_zpi_full(); break;
	case 0xd3: nop_c_imp_full(); break;
	case 0xd4: nop_s_zpx_full(); break;
	case 0xd5: cmp_s_zpx_full(); break;
	case 0xd6: dec_s_zpx_full(); break;
	case 0xd7: smb_bzp_full(); break;
	case 0xd8: cld_imp_full(); break;
	case 0xd9: cmp_s_aby_full(); break;
	case 0xda: phx_imp_full(); break;
	case 0xdb: stp_imp_full(); break;
	case 0xdc: nop_s_abx_full(); break;
	case 0xdd: cmp_s_abx_full(); break;
	case 0xde: dec_s_abx_full(); break;
	case 0xdf: bbs_zpb_full(); break;
	case 0xe0: cpx_imm_full(); break;
	case 0xe1: sbc_s_idx_full(); break;
	case 0xe2: nop_imm_full(); break;
	case 0xe3: nop_c_imp_full(); break;
	case 0xe4: cpx_zpg_full(); break;
	case 0xe5: sbc_c_zpg_full(); break;
	case 0xe6: inc_s_zpg_full(); break;
	case 0xe7: smb_bzp_full(); break;
	case 0xe8: inx_imp_full(); break;
	case 0xe9: sbc_c_imm_full(); break;
	case 0xea: nop_imp_full(); break;
	case 0xeb: nop_c_imp_full(); break;
	case 0xec: cpx_aba_full(); break;
	case 0xed: sbc_c_aba_full(); break;
	case 0xee: inc_c_aba_full(); break;
	case 0xef: bbs_zpb_full(); break;
	case 0xf0: beq_rel_full(); break;
	case 0xf1: sbc_s_idy_full(); break;
	case 0xf2: sbc_c_zpi_full(); break;
	case 0xf3: nop_c_imp_full(); break;
	case 0xf4: nop_s_zpx_full(); break;
	case 0xf5: sbc_s_zpx_full(); break;
	case 0xf6: inc_s_zpx_full(); break;
	case 0xf7: smb_bzp_full(); break;
	case 0xf8: sed_imp_full(); break;
	case 0xf9: sbc_s_aby_full(); break;
	case 0xfa: plx_s_imp_full(); break;
	case 0xfb: nop_c_imp_full(); break;
	case 0xfc: nop_s_abx_full(); break;
	case 0xfd: sbc_s_abx_full(); break;
	case 0xfe: inc_s_abx_full(); break;
	case 0xff: bbs_zpb_full(); break;
	case STATE_RESET: reset_s_full(); break;
	}
}

void w65c02s_device::do_exec_partial()
{
	switch(inst_state) {

	case 0x00: brk_s_imp_partial(); break;
	case 0x01: ora_s_idx_partial(); break;
	case 0x02: nop_imm_partial(); break;
	case 0x03: nop_c_imp_partial(); break;
	case 0x04: tsb_s_zpg_partial(); break;
	case 0x05: ora_zpg_partial(); break;
	case 0x06: asl_s_zpg_partial(); break;
	case 0x07: rmb_bzp_partial(); break;
	case 0x08: php_imp_partial(); break;
	case 0x09: ora_imm_partial(); break;
	case 0x0a: asl_acc_partial(); break;
	case 0x0b: nop_c_imp_partial(); break;
	case 0x0c: tsb_aba_partial(); break;
	case 0x0d: ora_aba_partial(); break;
	case 0x0e: asl_c_aba_partial(); break;
	case 0x0f: bbr_zpb_partial(); break;
	case 0x10: bpl_rel_partial(); break;
	case 0x11: ora_s_idy_partial(); break;
	case 0x12: ora_zpi_partial(); break;
	case 0x13: nop_c_imp_partial(); break;
	case 0x14: trb_s_zpg_partial(); break;
	case 0x15: ora_s_zpx_partial(); break;
	case 0x16: asl_s_zpx_partial(); break;
	case 0x17: rmb_bzp_partial(); break;
	case 0x18: clc_imp_partial(); break;
	case 0x19: ora_s_aby_partial(); break;
	case 0x1a: inc_acc_partial(); break;
	case 0x1b: nop_c_imp_partial(); break;
	case 0x1c: trb_aba_partial(); break;
	case 0x1d: ora_s_abx_partial(); break;
	case 0x1e: asl_s_abx_partial(); break;
	case 0x1f: bbr_zpb_partial(); break;
	case 0x20: jsr_adr_partial(); break;
	case 0x21: and_s_idx_partial(); break;
	case 0x22: nop_imm_partial(); break;
	case 0x23: nop_c_imp_partial(); break;
	case 0x24: bit_zpg_partial(); break;
	case 0x25: and_zpg_partial(); break;
	case 0x26: rol_s_zpg_partial(); break;
	case 0x27: rmb_bzp_partial(); break;
	case 0x28: plp_s_imp_partial(); break;
	case 0x29: and_imm_partial(); break;
	case 0x2a: rol_acc_partial(); break;
	case 0x2b: nop_c_imp_partial(); break;
	case 0x2c: bit_aba_partial(); break;
	case 0x2d: and_aba_partial(); break;
	case 0x2e: rol_c_aba_partial(); break;
	case 0x2f: bbr_zpb_partial(); break;
	case 0x30: bmi_rel_partial(); break;
	case 0x31: and_s_idy_partial(); break;
	case 0x32: and_zpi_partial(); break;
	case 0x33: nop_c_imp_partial(); break;
	case 0x34: bit_s_zpx_partial(); break;
	case 0x35: and_s_zpx_partial(); break;
	case 0x36: rol_s_zpx_partial(); break;
	case 0x37: rmb_bzp_partial(); break;
	case 0x38: sec_imp_partial(); break;
	case 0x39: and_s_aby_partial(); break;
	case 0x3a: dec_acc_partial(); break;
	case 0x3b: nop_c_imp_partial(); break;
	case 0x3c: bit_s_abx_partial(); break;
	case 0x3d: and_s_abx_partial(); break;
	case 0x3e: rol_s_abx_partial(); break;
	case 0x3f: bbr_zpb_partial(); break;
	case 0x40: rti_s_imp_partial(); break;
	case 0x41: eor_s_idx_partial(); break;
	case 0x42: nop_imm_partial(); break;
	case 0x43: nop_c_imp_partial(); break;
	case 0x44: nop_zpg_partial(); break;
	case 0x45: eor_zpg_partial(); break;
	case 0x46: lsr_s_zpg_partial(); break;
	case 0x47: rmb_bzp_partial(); break;
	case 0x48: pha_imp_partial(); break;
	case 0x49: eor_imm_partial(); break;
	case 0x4a: lsr_acc_partial(); break;
	case 0x4b: nop_c_imp_partial(); break;
	case 0x4c: jmp_adr_partial(); break;
	case 0x4d: eor_aba_partial(); break;
	case 0x4e: lsr_c_aba_partial(); break;
	case 0x4f: bbr_zpb_partial(); break;
	case 0x50: bvc_rel_partial(); break;
	case 0x51: eor_s_idy_partial(); break;
	case 0x52: eor_zpi_partial(); break;
	case 0x53: nop_c_imp_partial(); break;
	case 0x54: nop_s_zpx_partial(); break;
	case 0x55: eor_s_zpx_partial(); break;
	case 0x56: lsr_s_zpx_partial(); break;
	case 0x57: rmb_bzp_partial(); break;
	case 0x58: cli_imp_partial(); break;
	case 0x59: eor_s_aby_partial(); break;
	case 0x5a: phy_imp_partial(); break;
	case 0x5b: nop_c_imp_partial(); break;
	case 0x5c: nop_c_aba_partial(); break;
	case 0x5d: eor_s_abx_partial(); break;
	case 0x5e: lsr_s_abx_partial(); break;
	case 0x5f: bbr_zpb_partial(); break;
	case 0x60: rts_s_imp_partial(); break;
	case 0x61: adc_s_idx_partial(); break;
	case 0x62: nop_imm_partial(); break;
	case 0x63: nop_c_imp_partial(); break;
	case 0x64: stz_zpg_partial(); break;
	case 0x65: adc_c_zpg_partial(); break;
	case 0x66: ror_s_zpg_partial(); break;
	case 0x67: rmb_bzp_partial(); break;
	case 0x68: pla_s_imp_partial(); break;
	case 0x69: adc_c_imm_partial(); break;
	case 0x6a: ror_acc_partial(); break;
	case 0x6b: nop_c_imp_partial(); break;
	case 0x6c: jmp_s_ind_partial(); break;
	case 0x6d: adc_c_aba_partial(); break;
	case 0x6e: ror_c_aba_partial(); break;
	case 0x6f: bbr_zpb_partial(); break;
	case 0x70: bvs_rel_partial(); break;
	case 0x71: adc_s_idy_partial(); break;
	case 0x72: adc_c_zpi_partial(); break;
	case 0x73: nop_c_imp_partial(); break;
	case 0x74: stz_s_zpx_partial(); break;
	case 0x75: adc_s_zpx_partial(); break;
	case 0x76: ror_s_zpx_partial(); break;
	case 0x77: rmb_bzp_partial(); break;
	case 0x78: sei_imp_partial(); break;
	case 0x79: adc_s_aby_partial(); break;
	case 0x7a: ply_s_imp_partial(); break;
	case 0x7b: nop_c_imp_partial(); break;
	case 0x7c: jmp_s_iax_partial(); break;
	case 0x7d: adc_s_abx_partial(); break;
	case 0x7e: ror_s_abx_partial(); break;
	case 0x7f: bbr_zpb_partial(); break;
	case 0x80: bra_rel_partial(); break;
	case 0x81: sta_s_idx_partial(); break;
	case 0x82: nop_imm_partial(); break;
	case 0x83: nop_c_imp_partial(); break;
	case 0x84: sty_zpg_partial(); break;
	case 0x85: sta_zpg_partial(); break;
	case 0x86: stx_zpg_partial(); break;
	case 0x87: smb_bzp_partial(); break;
	case 0x88: dey_imp_partial(); break;
	case 0x89: bit_imm_partial(); break;
	case 0x8a: txa_imp_partial(); break;
	case 0x8b: nop_c_imp_partial(); break;
	case 0x8c: sty_aba_partial(); break;
	case 0x8d: sta_aba_partial(); break;
	case 0x8e: stx_aba_partial(); break;
	case 0x8f: bbs_zpb_partial(); break;
	case 0x90: bcc_rel_partial(); break;
	case 0x91: sta_s_idy_partial(); break;
	case 0x92: sta_zpi_partial(); break;
	case 0x93: nop_c_imp_partial(); break;
	case 0x94: sty_s_zpx_partial(); break;
	case 0x95: sta_s_zpx_partial(); break;
	case 0x96: stx_s_zpy_partial(); break;
	case 0x97: smb_bzp_partial(); break;
	case 0x98: tya_imp_partial(); break;
	case 0x99: sta_s_aby_partial(); break;
	case 0x9a: txs_imp_partial(); break;
	case 0x9b: nop_c_imp_partial(); break;
	case 0x9c: stz_aba_partial(); break;
	case 0x9d: sta_s_abx_partial(); break;
	case 0x9e: stz_s_abx_partial(); break;
	case 0x9f: bbs_zpb_partial(); break;
	case 0xa0: ldy_imm_partial(); break;
	case 0xa1: lda_s_idx_partial(); break;
	case 0xa2: ldx_imm_partial(); break;
	case 0xa3: nop_c_imp_partial(); break;
	case 0xa4: ldy_zpg_partial(); break;
	case 0xa5: lda_zpg_partial(); break;
	case 0xa6: ldx_zpg_partial(); break;
	case 0xa7: smb_bzp_partial(); break;
	case 0xa8: tay_imp_partial(); break;
	case 0xa9: lda_imm_partial(); break;
	case 0xaa: tax_imp_partial(); break;
	case 0xab: nop_c_imp_partial(); break;
	case 0xac: ldy_aba_partial(); break;
	case 0xad: lda_aba_partial(); break;
	case 0xae: ldx_aba_partial(); break;
	case 0xaf: bbs_zpb_partial(); break;
	case 0xb0: bcs_rel_partial(); break;
	case 0xb1: lda_s_idy_partial(); break;
	case 0xb2: lda_zpi_partial(); break;
	case 0xb3: nop_c_imp_partial(); break;
	case 0xb4: ldy_s_zpx_partial(); break;
	case 0xb5: lda_s_zpx_partial(); break;
	case 0xb6: ldx_s_zpy_partial(); break;
	case 0xb7: smb_bzp_partial(); break;
	case 0xb8: clv_imp_partial(); break;
	case 0xb9: lda_s_aby_partial(); break;
	case 0xba: tsx_imp_partial(); break;
	case 0xbb: nop_c_imp_partial(); break;
	case 0xbc: ldy_s_abx_partial(); break;
	case 0xbd: lda_s_abx_partial(); break;
	case 0xbe: ldx_s_aby_partial(); break;
	case 0xbf: bbs_zpb_partial(); break;
	case 0xc0: cpy_imm_partial(); break;
	case 0xc1: cmp_s_idx_partial(); break;
	case 0xc2: nop_imm_partial(); break;
	case 0xc3: nop_c_imp_partial(); break;
	case 0xc4: cpy_zpg_partial(); break;
	case 0xc5: cmp_zpg_partial(); break;
	case 0xc6: dec_s_zpg_partial(); break;
	case 0xc7: smb_bzp_partial(); break;
	case 0xc8: iny_imp_partial(); break;
	case 0xc9: cmp_imm_partial(); break;
	case 0xca: dex_imp_partial(); break;
	case 0xcb: wai_imp_partial(); break;
	case 0xcc: cpy_aba_partial(); break;
	case 0xcd: cmp_aba_partial(); break;
	case 0xce: dec_c_aba_partial(); break;
	case 0xcf: bbs_zpb_partial(); break;
	case 0xd0: bne_rel_partial(); break;
	case 0xd1: cmp_s_idy_partial(); break;
	case 0xd2: cmp_zpi_partial(); break;
	case 0xd3: nop_c_imp_partial(); break;
	case 0xd4: nop_s_zpx_partial(); break;
	case 0xd5: cmp_s_zpx_partial(); break;
	case 0xd6: dec_s_zpx_partial(); break;
	case 0xd7: smb_bzp_partial(); break;
	case 0xd8: cld_imp_partial(); break;
	case 0xd9: cmp_s_aby_partial(); break;
	case 0xda: phx_imp_partial(); break;
	case 0xdb: stp_imp_partial(); break;
	case 0xdc: nop_s_abx_partial(); break;
	case 0xdd: cmp_s_abx_partial(); break;
	case 0xde: dec_s_abx_partial(); break;
	case 0xdf: bbs_zpb_partial(); break;
	case 0xe0: cpx_imm_partial(); break;
	case 0xe1: sbc_s_idx_partial(); break;
	case 0xe2: nop_imm_partial(); break;
	case 0xe3: nop_c_imp_partial(); break;
	case 0xe4: cpx_zpg_partial(); break;
	case 0xe5: sbc_c_zpg_partial(); break;
	case 0xe6: inc_s_zpg_partial(); break;
	case 0xe7: smb_bzp_partial(); break;
	case 0xe8: inx_imp_partial(); break;
	case 0xe9: sbc_c_imm_partial(); break;
	case 0xea: nop_imp_partial(); break;
	case 0xeb: nop_c_imp_partial(); break;
	case 0xec: cpx_aba_partial(); break;
	case 0xed: sbc_c_aba_partial(); break;
	case 0xee: inc_c_aba_partial(); break;
	case 0xef: bbs_zpb_partial(); break;
	case 0xf0: beq_rel_partial(); break;
	case 0xf1: sbc_s_idy_partial(); break;
	case 0xf2: sbc_c_zpi_partial(); break;
	case 0xf3: nop_c_imp_partial(); break;
	case 0xf4: nop_s_zpx_partial(); break;
	case 0xf5: sbc_s_zpx_partial(); break;
	case 0xf6: inc_s_zpx_partial(); break;
	case 0xf7: smb_bzp_partial(); break;
	case 0xf8: sed_imp_partial(); break;
	case 0xf9: sbc_s_aby_partial(); break;
	case 0xfa: plx_s_imp_partial(); break;
	case 0xfb: nop_c_imp_partial(); break;
	case 0xfc: nop_s_abx_partial(); break;
	case 0xfd: sbc_s_abx_partial(); break;
	case 0xfe: inc_s_abx_partial(); break;
	case 0xff: bbs_zpb_partial(); break;
	case STATE_RESET: reset_s_partial(); break;
	}
}

