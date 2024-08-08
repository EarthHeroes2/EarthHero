#include "PerkInformation.h"

PerkInformation::PerkInformation()
{
	PerkDescriptions.SetNum(NumOfPerks);
	
	PerkDescriptions[0].Name = FText::FromString(TEXT("묵직한 일격 : 알파"));
	PerkDescriptions[1].Name = FText::FromString(TEXT("궤멸적 타격 : 알파"));
	PerkDescriptions[2].Name = FText::FromString(TEXT("재생성 피부 : 알파"));
	PerkDescriptions[3].Name = FText::FromString(TEXT("스피드 스타 : 알파"));
	PerkDescriptions[4].Name = FText::FromString(TEXT("정교한 기술 : 알파"));
	
	PerkDescriptions[5].Name = FText::FromString(TEXT("추격자"));
	PerkDescriptions[6].Name = FText::FromString(TEXT("안정감"));
	PerkDescriptions[7].Name = FText::FromString(TEXT("치명상"));
	PerkDescriptions[8].Name = FText::FromString(TEXT("공중 도약"));
	PerkDescriptions[9].Name = FText::FromString(TEXT("서스테인 배리어"));

	PerkDescriptions[10].Name = FText::FromString(TEXT("산뜻한 시작"));
	PerkDescriptions[11].Name = FText::FromString(TEXT("소닉 스트라이크"));
	PerkDescriptions[12].Name = FText::FromString(TEXT("슬로우 스타터"));
	PerkDescriptions[13].Name = FText::FromString(TEXT("열기"));
	PerkDescriptions[14].Name = FText::FromString(TEXT("사신"));

	PerkDescriptions[15].Name = FText::FromString(TEXT("제너럴리스트"));
	PerkDescriptions[16].Name = FText::FromString(TEXT("스페셜리스트"));
	PerkDescriptions[17].Name = FText::FromString(TEXT("확장된 사고"));
	PerkDescriptions[18].Name = FText::FromString(TEXT("유망한 학자"));
	PerkDescriptions[19].Name = FText::FromString(TEXT("대기만성"));

	PerkDescriptions[20].Name = FText::FromString(TEXT("기선 제압"));
	PerkDescriptions[21].Name = FText::FromString(TEXT("통제 저항"));
	PerkDescriptions[22].Name = FText::FromString(TEXT("결집된 의지"));
	PerkDescriptions[23].Name = FText::FromString(TEXT("외로운 늑대"));
	PerkDescriptions[24].Name = FText::FromString(TEXT("클러치 히터"));

	PerkDescriptions[25].Name = FText::FromString(TEXT("묵직한 일격 : 베타"));
	PerkDescriptions[26].Name = FText::FromString(TEXT("궤멸적 타격 : 베타"));
	PerkDescriptions[27].Name = FText::FromString(TEXT("재생성 피부 : 베타"));
	PerkDescriptions[28].Name = FText::FromString(TEXT("스피드 스타 : 베타"));
	PerkDescriptions[29].Name = FText::FromString(TEXT("정교한 기술 : 베타"));

	PerkDescriptions[30].Name = FText::FromString(TEXT("스타터 배리어"));
	PerkDescriptions[31].Name = FText::FromString(TEXT("얼리 어답터"));
	PerkDescriptions[32].Name = FText::FromString(TEXT("아드레날린"));
	PerkDescriptions[33].Name = FText::FromString(TEXT("유연한 사고"));
	PerkDescriptions[34].Name = FText::FromString(TEXT("경직된 사고"));

	PerkDescriptions[35].Name = FText::FromString(TEXT("윙맨"));
	PerkDescriptions[36].Name = FText::FromString(TEXT("긴급 상황 탈출기"));
	PerkDescriptions[37].Name = FText::FromString(TEXT("드레인 머신"));
	PerkDescriptions[38].Name = FText::FromString(TEXT("바운티 헌터"));
	PerkDescriptions[39].Name = FText::FromString(TEXT("쉴드 오버로드"));

	PerkDescriptions[40].Name = FText::FromString(TEXT("엣지 러너"));
	PerkDescriptions[41].Name = FText::FromString(TEXT("애프터번"));
	PerkDescriptions[42].Name = FText::FromString(TEXT("프라이멀 에너지"));
	PerkDescriptions[43].Name = FText::FromString(TEXT("아이언 하트"));
	PerkDescriptions[44].Name = FText::FromString(TEXT("하이퍼 배리어"));

	PerkDescriptions[45].Name = FText::FromString(TEXT("H 코드 : 마에스트로"));
	PerkDescriptions[46].Name = FText::FromString(TEXT("H 코드 : 컨트롤러"));
	PerkDescriptions[47].Name = FText::FromString(TEXT("H 코드 : 캡틴"));
	PerkDescriptions[48].Name = FText::FromString(TEXT("H 코드 : 스트라이커"));
	PerkDescriptions[49].Name = FText::FromString(TEXT("H 코드 : 버스터"));
	
	
	for(int i = 0; i < NumOfPerks; i++)
		PerkDescriptions[i].Cost = FText::FromString(FString::FromInt(NeedPoint[i]));

	
	PerkDescriptions[0].Description = FText::FromString(TEXT("일반 공격 대미지 +48%"));
	PerkDescriptions[1].Description = FText::FromString(TEXT("공격 스킬 대미지 +90%"));
	PerkDescriptions[2].Description = FText::FromString(TEXT("체력 재생(초) +120%"));
	PerkDescriptions[3].Description = FText::FromString(TEXT("일반 공격 속도 +35%, 이동 속도 +17%"));
	PerkDescriptions[4].Description = FText::FromString(TEXT("공격 스킬 쿨타임 -7%, 이동 스킬 쿨타임 -10%"));

	PerkDescriptions[5].Description = FText::FromString(TEXT("일정 범위 내 적에게 다가갈 때, 이동 속도 ×1.25"));
	PerkDescriptions[6].Description = FText::FromString(TEXT("최대 체력의 80% 이상의 체력을 보유 중일 때, 일반 공격 대미지 +25%, 공격 스킬 대미지 +50%."));
	PerkDescriptions[7].Description = FText::FromString(TEXT("모든 종류의 내가 입히는 대미지가 10% 확률로 ×3의 피해."));
	PerkDescriptions[8].Description = FText::FromString(TEXT("점프력 +20%, 더블 점프 해금."));
	PerkDescriptions[9].Description = FText::FromString(TEXT("최대 체력 25% 보호막 획득. 보호막 소진되면 8초 후 재생성."));

	PerkDescriptions[10].Description = FText::FromString(TEXT("세션 시작하고 3분간, 이동 속도 +30%, 체력 재생 (초) +100%"));
	PerkDescriptions[11].Description = FText::FromString(TEXT("이동 속도 상승치 20%만큼 일반 공격 & 공격 스킬 대미지 상승. ( 예 : 합/곱연산 후 이동 속도 상승치가 150%면, 30%씩 상승. )"));
	PerkDescriptions[12].Description = FText::FromString(TEXT("세션 시작하고 8분 후 부터, 일반 공격 대미지 +60%, 공격 스킬 대미지 +120%"));
	PerkDescriptions[13].Description = FText::FromString(TEXT("일반 공격 유지하면 (LB 계속 누르고 있으면) 일반 공격 속도 1초마다 +40% 증가하여, 3초 후 최대 +120% (LB 떼면은 취소)"));
	PerkDescriptions[14].Description = FText::FromString(TEXT("체력이 33% 이하로 남은 적에게 ×4의 모든 종류의 피해."));

	PerkDescriptions[15].Description = FText::FromString(TEXT("히어로 업그레이드 [공통] 등장 확률 2배 증가."));
	PerkDescriptions[16].Description = FText::FromString(TEXT("히어로 업그레이드 [클래스 전용] 등장 확률 2배 증가."));
	PerkDescriptions[17].Description = FText::FromString(TEXT("히어로 업그레이드 3개가 아닌 4개 중 선택. (키패드 4로 선택)"));
	PerkDescriptions[18].Description = FText::FromString(TEXT("히어로 업그레이드 Lv. 경험치 획득량 +25%"));
	PerkDescriptions[19].Description = FText::FromString(TEXT("최대 Lv.+2 (12까지 업할 수 있고, 11/12 EXP 요구치는 10과 동일.)"));

	PerkDescriptions[20].Description = FText::FromString(TEXT("체력이 100%인 적에게 ×1.25의 모든 종류의 피해."));
	PerkDescriptions[21].Description = FText::FromString(TEXT("자기장으로 인해 입는 피해 -60%, 자기장 범위 내에서 이동 속도 +50%"));
	PerkDescriptions[22].Description = FText::FromString(TEXT("가까운 범위 내에 플레이어가 3명 이상일 때, 최대 체력 +90%, 체력 재생(초) + 150%, 이동 속도 +45%, 이동 스킬 쿨타임 -12%"));
	PerkDescriptions[23].Description = FText::FromString(TEXT("넓은 범위 내에 플레이어가 본인 혼자일 때, 일반 공격 대미지 +60%, 일반 공격 속도 +45%, 공격 스킬 대미지 +100% 공격 스킬 쿨타임 -10%"));
	PerkDescriptions[24].Description = FText::FromString(TEXT("세션 시작하고 15분 후 부터, 일반 공격 대미지 : +40%. 일반 공격 속도 : +30%, 공격 스킬 대미지 : +75%, 공격 스킬 쿨타임 : -6%, 이동 스킬 쿨타임 : -8%, 최대 체력 : +60%, 체력 재생(초) : +100%, 이동 속도 : +30%"));

	PerkDescriptions[25].Description = FText::FromString(TEXT("일반 공격 대미지 +92%"));
	PerkDescriptions[26].Description = FText::FromString(TEXT("공격 스킬 대미지 +173%"));
	PerkDescriptions[27].Description = FText::FromString(TEXT("체력 재생(초) +230%"));
	PerkDescriptions[28].Description = FText::FromString(TEXT("일반 공격 속도 +65%, 이동 속도 +32%"));
	PerkDescriptions[29].Description = FText::FromString(TEXT("공격 스킬 쿨타임 -12%, 이동 스킬 쿨타임 -18%"));

	PerkDescriptions[30].Description = FText::FromString(TEXT("시작할 때, 최대 체력 40% 보호막 획득. 본 보호막은 소진되면 재생성되지 않음."));
	PerkDescriptions[31].Description = FText::FromString(TEXT("세션 시작할 때, Lv.3으로 시작."));
	PerkDescriptions[32].Description = FText::FromString(TEXT("적을 처치하면, 아드레날린 1스택 상승. (최대 50스택). 1.5초 내에 적을 처치하지 못하면, 아드레날린 스택 리셋. 아드레날린 1스택당, 일반 공격 속도 +1% / 이동 속도 +1%."));
	PerkDescriptions[33].Description = FText::FromString(TEXT("키패드 5를 누르면 히어로 업그레이드 1회 Reroll."));
	PerkDescriptions[34].Description = FText::FromString(TEXT("히어로 업그레이드 고정적으로 1개만 등장. (본래 3개든, 4개든.) Reroll은 할 수 있음. 즉 선택할 수 없고, 나온대로 1번 눌러 업. 대신 1Lv. UP당 2회 업그레이드 할 수 있음."));

	PerkDescriptions[35].Description = FText::FromString(TEXT("점프 중 이동 속도 +45%."));
	PerkDescriptions[36].Description = FText::FromString(TEXT("현재 체력이 최대 체력의 10% 이하가 되는 피해를 입기 직전, 2초간 무적 상태 및 이동 속도 +100%. (쿨타임 60초)"));
	PerkDescriptions[37].Description = FText::FromString(TEXT("일정 범위 내에서 적이 사망할 때마다, 내 최대 체력 비례 회복. 가까운 범위 내 : 내  최대 체력의 0.6% 회복. 넓은 범위 내 : 내 최대 체력의 0.2% 회복"));
	PerkDescriptions[38].Description = FText::FromString(TEXT("보스 상대로 모든 종류의 대미지 ×(1.4 +처치 기여 보스 수 ×0.2). 보스가 사망하기 전에, 1대라도 대미지 입혔으면 기여 판정."));
	PerkDescriptions[39].Description = FText::FromString(TEXT("보호막 보유 중인 상태일 때, 일반 공격 대미지 +80%. 일반 공격 속도 +60%"));

	PerkDescriptions[40].Description = FText::FromString(TEXT("보유한 체력이 최대 체력의 50%보다 이하일 때, 일반 공격 속도 +70%. 이동 속도 +70%."));
	PerkDescriptions[41].Description = FText::FromString(TEXT("공격 스킬 사용한 뒤, 0.5초마다 일정 범위 내에 대미지 주는 플레이어 중심으로 화염 오오라를 2초간 생성. 대미지는, 공격 스킬 대미지의 60%."));
	PerkDescriptions[42].Description = FText::FromString(TEXT("이동 스킬 시전 직후, 3초간 체력 재생(초) : +300%"));
	PerkDescriptions[43].Description = FText::FromString(TEXT("일반 공격 대미지 +100%, 공격 스킬 대미지 +200%, 최대 체력 +150%, 어떤 방법으로든 체력을 회복할 수 없게 됨."));
	PerkDescriptions[44].Description = FText::FromString(TEXT("최대 체력 50% 보호막 획득. 보호막 소진되면 25초 후 재생성."));

	PerkDescriptions[45].Description = FText::FromString(TEXT("※ H 코드는 중복해서 장착할 수 없고, 1개만 장착 가능. (F)를 눌러, 가장 가까운 아군 2초간 배리어 부여. (쿨타임 8초) 부여 받은 아군은 2초간 최대 체력의 10%만큼 보호막 부여받고, 2초간 체력 재생(초) +250% (일정 범위 내 아군 없으면 나에게)"));
	PerkDescriptions[46].Description = FText::FromString(TEXT("※ H 코드는 중복해서 장착할 수 없고, 1개만 장착 가능. (F)를 눌러, 주변 모든 적군을 0.5초간 스턴. (쿨타임 10초)"));
	PerkDescriptions[47].Description = FText::FromString(TEXT("※ H 코드는 중복해서 장착할 수 없고, 1개만 장착 가능. (F)를 눌러, 6초간 자신의 일반 공격 속도 +40%, 이동 속도 +40% (쿨타임 15초), 일정 범위 내 모든 아군은 절반의 효과 받음."));
	PerkDescriptions[48].Description = FText::FromString(TEXT("※ H 코드는 중복해서 장착할 수 없고, 1개만 장착 가능. (F)를 눌러, 공격 스킬 쿨 리셋, 다음 공격 스킬 강화. (쿨타임 8초) 강화된 공격 스킬은 적이 보유한 체력에 비례해 최대 ×3 피해. ( 보유 체력 : 25% → ×1.5 / 50% → ×2 / 75% → ×2.5 )"));
	PerkDescriptions[49].Description = FText::FromString(TEXT("※ H 코드는 중복해서 장착할 수 없고, 1개만 장착 가능. (F)를 눌러, 다음 1.5초간의 일반 공격 강화.  (쿨타임 9초) 강화된 일반 공격은 적이 잃은 체력에 비례해 최대 ×5 피해. ( 잃은 체력 :  25% → ×2 / 50% → ×3 / 75% → ×4 )"));
}

void PerkInformation::SetImages()
{
	UTexture2D* ImagePath = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("Texture2D'/Game/Assets/Textures/Void_Heart.Void_Heart'")));
	for(int i=0; i < 50; i++)
	{
		PerkDescriptions[i].Image->SetBrushFromTexture(ImagePath);
	}
}

//이거 현재 안씀 (level + 2로 그냥 코딩되어있음)
int PerkInformation::GetPoints(int Level)
{
	return Level + 2;
}
