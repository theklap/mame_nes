void m6509_device::lda_9_idy_full()
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
	PC++;
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
		read_9(set_l(TMP, TMP+Y));
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
	A = read_9(TMP+Y);
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

void m6509_device::lda_9_idy_partial()
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
	PC++;
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
		read_9(set_l(TMP, TMP+Y));
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
	A = read_9(TMP+Y);
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

void m6509_device::sta_9_idy_full()
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
	PC++;
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
	read_9(set_l(TMP, TMP+Y));
	icount--;
	if(icount <= 0) {
		if(access_to_be_redone()) {
			icount++;
			inst_substate = 7;
		} else
			inst_substate = 8;
		return;
	}
	write_9(TMP+Y, A);
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

void m6509_device::sta_9_idy_partial()
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
	PC++;
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
	read_9(set_l(TMP, TMP+Y));
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
	write_9(TMP+Y, A);
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



void m6509_device::do_exec_full()
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
	case 0x91: sta_9_idy_full(); break;
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
	case 0xb1: lda_9_idy_full(); break;
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

void m6509_device::do_exec_partial()
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
	case 0x91: sta_9_idy_partial(); break;
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
	case 0xb1: lda_9_idy_partial(); break;
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

