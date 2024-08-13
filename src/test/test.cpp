/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc_kvm.h"
#include"lib/log.h"
#include"lib/exception.h"
#include"test.h"

struct UnitTestCollection{
	std::vector<UnitTest*>tests;
};
static UnitTestCollection*tests=nullptr;

void UnitTest::RegisterSelf(){
	if(!tests)tests=new UnitTestCollection;
	tests->tests.push_back(this);
}

int RunUnitTest(){
	size_t total=0,success=0,failed=0;
	if(!tests)throw RuntimeError("unit test is not initialized");
	for(auto test:tests->tests){
		auto name=test->GetName();
		try{
			log_info("Running test {}...",name);
			test->RunTest();
			success++;
		}catch(std::exception&exc){
			log_warn("Test {} failed: {}",name,exc.what());
			failed++;
		}
		total++;
	}
	log_info(
		"Run {} tests, {} success, {} failed ({}%)",
		total,success,failed,success*100/total
	);
	return failed==0?0:1;
}
