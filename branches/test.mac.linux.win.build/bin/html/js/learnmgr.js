var EnumTypeJudge = {
    ETJNone: 0,
    ETJKnowwell: 1, // 掌握
    ETJPlain: 2,    // 记得
    ETJDim: 3,      // 模糊
    ETJOblivious: 4,    // 忘记
    ETJUnknown: 5       // 未知
}

var EnumTypeAffirm = {
    ETANone: 0,
    ETAYes: 1, // 正确
    ETANo: 2    // 错误
}
var special_keys = {
    27: 'esc', 9: 'tab', 32: 'space', 13: 'return', 8: 'backspace', 145: 'scroll', 20: 'capslock',
    144: 'numlock', 19: 'pause', 45: 'insert', 36: 'home', 46: 'del', 35: 'end', 33: 'pageup',
    34: 'pagedown', 37: 'left', 38: 'up', 39: 'right', 40: 'down', 112: 'f1', 113: 'f2', 114: 'f3',
    115: 'f4', 116: 'f5', 117: 'f6', 118: 'f7', 119: 'f8', 120: 'f9', 121: 'f10', 122: 'f11', 123: 'f12', 188: ',', 190: '.'
};
function StudyInfo() {
    this.mid = 0;
    this.stime = 0;
    this.judgetime = 0;
    this.affirmtime = 0;
    this.studytime = 0;
    this.selJudge = EnumTypeJudge.ETJNone;
    this.selAffirm = EnumTypeAffirm.ETANone;

    this.reset = function() {
        this.mid = 0;
        this.stime = 0;
        this.judgetime = 0;
        this.affirmtime = 0;
        this.studytime = 0;
        this.selJudge = EnumTypeJudge.ETJNone;
        this.selAffirm = EnumTypeAffirm.ETANone;
    }
};

g_statestarttime = 0;    // 某状态开始时间
g_odlestarttime = 0; // 超时开始时间
g_overtimeCount = 0; // 超时时间计数
g_idlestarttime = 0;
g_sinfo = new StudyInfo();
g_dictinfo = new DictInfo();
var cwordinfo;

bwingshow = true;
bdmeanshow = true;
bemeanshow = true;
bmcodeshow = true;
bthesaurusshow = true;
bsyllableshow = true;
bsuppletionshow = true;
bsensshow = true;
bgnameshow = true;
nowlm = "lmsimple"
dictsel = "#langisle";
bshowShortcutKeyTip = true;
var skillsel = "e_c";
var nowUseSkill = "";

var AryTL;

var keySet = {};
var oldkeySet = {};
var currentSelWord;

$(document).ready(function() {
    //learnMgr.updateCurBookInfo.connect(updateCurBookInfo);
    //learnMgr.updateCurWordBaseInfo.connect(updateCurWordBaseInfo);
		//$("body").css("Display", "none");
		
    $("#fullScreen").attr("checked", learnMgr.getConfigInfoBool("LearnMgr/fullScreen", false));

    jkmegamenu.definemenu("learnoptionbutton", "learnoption", "mouseover")
    learnMgr.updateSTime.connect(updateSTime);

    // learnMgr.init();
    //$("#tabdict ul").idTabs();
    nowlm = getNowLM();
    //log("#" + nowlm);
    $("#" + nowlm).attr("checked", true);

    g_dictinfo.setInfo(learnMgr.getDictInfo());
    g_dictinfo.setIdTables();
    $("#tabdict ul").idTabs(idTabsHandle);
    $("#learnoptiontab ul").idTabs();


    // 快捷键提示
    bshowShortcutKeyTip = learnMgr.getConfigInfoBool("lo_other", false);
    $("#showShortcutKeyTip").attr('checked', bshowShortcutKeyTip);
    // 播放次数
    $("#replaytimes ").val(learnMgr.getReplaytimes());
    $("#replaytimes").change(replaytimesChange);
    // 播放间隔
    $("#replaysleep ").val(learnMgr.getReplaySleep());
    $("#replaysleep").change(replaysleepChange);
    // 当前学习技能
    $("#skill").val(learnMgr.getConfigInfo("LearnMgr/skill", 'e_c'));
    $("#skill").change(skillChange);
    skillChange();
    // 当前出词顺序
    $("#gWordOrder").val(learnMgr.getConfigInfo("LearnMgr/gWordOrder", 'a_b_c'));
    $("#rvWordOrder").val(learnMgr.getConfigInfo("LearnMgr/rvWordOrder", 'a_b_c'));
    $("#gWordOrder").change(gWordOrderChange);
    $("#rvWordOrder").change(rvWordOrderChange);
    gWordOrderChange();
    rvWordOrderChange();

    // $("#tab ul").idTabs(1);
    jkmegamenu.render($);
    initStudyMode(1);   // 普通模式

    lmInit();
    initainfo();
    var thotkey = learnMgr.getConfigInfo("LearnMgr/Hotkey", 'wesdxc');
    setHotScheme(thotkey);  // 快捷键
    $("#" + thotkey).attr('checked', true);

    updateCurBookInfo(learnMgr.getCurrentLearnBookInfo());
    nextWord();
    initTimer();
    reinitui();
    //$('#lo_learnmode *').tooltip();
    document.addEventListener("mouseup", lookup, false);
    document.addEventListener("mousedown", giveup, false);
    $("#selwordbox")[0].addEventListener("mouseup", stop, false);
    $("#selwordbox")[0].addEventListener("mousedown", stop, false);
    $("#min_close")[0].addEventListener("click", giveup, false);
    updatenewwordbooklist();

		
    $(document).bind('keydown', keydwonHandler);


	$("#body").show();
	


});

function disableHotkeyTitle(keySet) {
    if (bshowShortcutKeyTip) {
        $("#Knowwell").attr("title", "快捷键:" + keySet.ETJKnowwell);
        $("#Plain").attr("title", "快捷键:" + keySet.ETJPlain);
        $("#AYes").attr("title", "快捷键:" + keySet.ETAYes);
        $("#SNext").attr("title", "快捷键:" + keySet.ETNext);
        $("#Dim").attr("title", "快捷键:" + keySet.ETJDim);
        $("#Oblivious").attr("title", "快捷键:" + keySet.ETJOblivious);
        $("#ANo").attr("title", "快捷键:" + keySet.ETANo);
        $("#Unknown").attr("title", "快捷键:" + keySet.ETJUnknown);
        $("#playvoice").attr("title", "快捷键:" + keySet.ETReplay);
        
    }
    else {
        $("#Knowwell").removeAttr("title");
        $("#Plain").removeAttr("title");
        $("#AYes").removeAttr("title");
        $("#SNext").removeAttr("title");
        $("#Dim").removeAttr("title");
        $("#Oblivious").removeAttr("title");
        $("#ANo").removeAttr("title");
        $("#Unknown").removeAttr("title");
        $("#playvoice").removeAttr("title");
    }
    $('*').tooltip();
    

}
function unbinkey() {
    try {
        $(document).unbind('keydown', oldkeySet.ETJKnowwell, 0);
        $(document).unbind('keydown', oldkeySet.ETJPlain, 0);
        $(document).unbind('keydown', oldkeySet.ETJDim, 0);
        $(document).unbind('keydown', oldkeySet.ETJOblivious, 0);
        $(document).unbind('keydown', oldkeySet.ETJUnknown, 0);
        $(document).unbind('keydown', oldkeySet.ETAYes, 0);
        $(document).unbind('keydown', oldkeySet.ETANo, 0);
        $(document).unbind('keydown', oldkeySet.ETNext, 0);
        $(document).unbind('keydown', oldkeySet.ETReplay, 0);
        /*$(document).unbind('keydown', keydwonHandler);*/
    }
    catch (err) {
        //alert("unbinkey err:" + err);
    }
    $(document).bind('keydown', keydwonHandler);
}

function resetHotkey(keySet) {

    //////////////////////////////////////////////////////////////////////////
    // 注意 jq1.4后的版本有问题,按一个键调用多次   
    //      lsJudge = 1;
    // lsAffirm = 2;
    // lsStudy = 3;
    //log("+++++++1");
    unbinkey();
	//log("+++++++2");
	try{
		    $(document).bind('keydown', keySet.ETAYes, function(e) {
        if (learnstate == lsAffirm && ("l_w" != nowUseSkill && "p_w" != nowUseSkill)) {
            clickAYes();
            return true;    //break;
        }

    });
		}
		catch(err)
		{
				alert("unbinkey 2:" + err);
		}

    //log("+++++++3");
    $(document).bind('keydown', keySet.ETANo, function() {
        if (learnstate == lsAffirm && ("l_w" != nowUseSkill && "p_w" != nowUseSkill)) {

            clickANo();
            return true;    //break;
        } 
    });

    $(document).bind('keydown', keySet.ETReplay, function() {
        if (("l_w" != nowUseSkill && "p_w" != nowUseSkill)) {

            clickSpeak();
            return true;    //break;
        }
    });

    $(document).bind('keydown', keySet.ETJKnowwell, function() {
        if (learnstate == lsJudge && nowlm == 'lmstandard' && ("l_w" != nowUseSkill && "p_w" != nowUseSkill)) {

            clickKnowwell();
            return true;    //break;
        }
    });
    $(document).bind('keydown', keySet.ETJDim, function() {
        if (learnstate == lsJudge && nowlm == 'lmstandard' && ("l_w" != nowUseSkill && "p_w" != nowUseSkill)) {

            clickDim();
            return true;    //break;
        }
    });
    $(document).bind('keydown', keySet.ETJUnknown, function() {
        if (learnstate == lsJudge && nowlm == 'lmstandard' && ("l_w" != nowUseSkill && "p_w" != nowUseSkill)) {

            clickUnknown();
            return true;    //break;
        }
    });
	

    $(document).bind('keydown', keySet.ETJPlain, function(e) {
        if (learnstate == lsJudge && ("l_w" != nowUseSkill && "p_w" != nowUseSkill)) {

            clickPlain();
            return true;    //break;
        }

    });

    $(document).bind('keydown', keySet.ETJOblivious, function() {
        if (learnstate == lsJudge && ("l_w" != nowUseSkill && "p_w" != nowUseSkill)) {
            clickOblivious();
            return true;    //break;
        }
    });



    //alert(keySet.ETNext);
    $(document).bind('keydown', keySet.ETNext, function(e) {
        if (learnstate == lsStudy && ("l_w" != nowUseSkill && "p_w" != nowUseSkill)) {
            /*$("#bSNext").trigger("click");*/
            /*$("#bSNext").click();*/


            clickSNext();
            return true;    //break;
        }
    });


       
    disableHotkeyTitle(keySet);
}

function reinitui() {
    //$("#respeak").attr("checked", learnMgr.IsLoopPlay());
    $("#gwrandom").attr("checked", learnMgr.IsGWRandom());
    filllo_learnui();
}


function clickSpeak() {
    // learnMgr.logOut("clickSpeak");
    learnMgr.speak(cwordinfo.word);

}

function nextWord() {
    updateLastWord();
    updateSTATSInfo();
    starttime = new Date().getTime();
    ret = learnMgr.getNextWord();
    if (ret < 0) {
        clickgoback();
        return;
    } else {
        // alert("LearnMgr.getNextWord取词成功, 代码" + ret);
        mid = ret
        
        $(".question").hide();
        $(".answer").hide();
        $(".ainfo").hide();
        cwordinfo = learnMgr.getCWordInfo(mid);
        //log(cwordinfo.wimg);
        updateCurWordBaseInfo(cwordinfo);
        g_sinfo.reset();
        g_sinfo.mid = mid;

        $("#dictloadding").hide();
        addr = g_dictinfo.getAddrByID(dictsel);
        if (addr) {
            // $(dictsel + " iframe").attr("src", "waitiframe.html?url=.html?url=" + addr.replace(/\[LIWord\]/i, cwordinfo.word));
            updateDict(addr, dictsel);


        }

//         log("#" + dictsel + " iframe");
//         $(dictsel + " iframe").attr("src", g_dictinfo.getAddrByID(dictsel));
//         $(dictsel + " iframe").attr("src", "../waitiframe.html?url=" + addr.replace(/\[LIWord\]/i, cwordinfo.word)); 
//         if (dictsel == g_dictinfo)
//             $("#youdao iframe").attr("src", "http://dict.youdao.com/m/search?keyfrom=dict.mindex&vendor=%24vendor&q=" + cwordinfo.word);
//         else if (dictsel == "#dictcn")
        //             $("#dictcn iframe").attr("src", "http://dict.cn/mini.php?q=" + cwordinfo.word);

        toInitState();
        
    }

    timeCount = new Date().getTime() - starttime;
    g_sinfo.stime = new Date().getTime() / 1000;

    learnMgr.logOut("取词用时" + timeCount);
}

function toInitState() {
    if (nowUseSkill != skillsel) {
        nowUseSkill = skillsel;
      
    };

    toJudgePage();
    if (nowUseSkill == "p_w" || nowUseSkill == "l_w") {
        $("#operate").hide();
        $("#otip .judge").html("请拼写单词,回车键确认.");
        $("#otip .study").html("请努力学习并记忆,之后按\"回车键\"继续学习.");
        $("#iword").focus();
    }
    else {
        $("#operate").show();
        $("#otip .judge").html("您记得该单词吗?");
        $("#otip .study").html("请努力学习并记忆,之后点击\"下一个\"继续学习.");
        $("#iword").blur();
    }    

}

function initStudyMode(mode) {
    if (mode == 1) {
        $(".question span.mean").hide();

        $(".answer span.word").hide();
        $(".answer span.pron").hide();
        
    }    
}

function lmInit() {
    if (nowlm == "lmsimple") {
        $("#Knowwell").hide();
        $("#Dim").hide();
        $("#Unknown").hide();
    }
    else if (nowlm = "lmstandard") 
    {
        $("#Knowwell").show();
        $("#Dim").show();
        $("#Unknown").show();   
    }    
}

//////////////////////////////////////////////////////////////////////////
// 学习
function clickSNext() {
    log("clickSNext 1");
    g_sinfo.studytime = getEffectiveTime();
    learnMgr.updateMInfo(g_sinfo);
    nextWord();
    log("clickSNext end");
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 确认
function clickAYes() {

    g_sinfo.affirmtime = getEffectiveTime();
    g_sinfo.selAffirm = EnumTypeAffirm.ETAYes;
    learnMgr.updateMInfo(g_sinfo);
    nextWord();
}

function clickANo() {
    
    g_sinfo.affirmtime = getEffectiveTime();
    g_sinfo.selAffirm = EnumTypeAffirm.ETANo;
    toStudyPage();
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 判断
function clickKnowwell() {

    g_sinfo.judgetime = getEffectiveTime();
    g_sinfo.selJudge = EnumTypeJudge.ETJKnowwell;
//     learnMgr.logOut(g_sinfo.judgetime + ',' + g_overtimeCount);
    learnMgr.updateMInfo(g_sinfo);
    nextWord();
}

function clickPlain() {

    g_sinfo.judgetime = getEffectiveTime();
    g_sinfo.selJudge = EnumTypeJudge.ETJPlain;
    toAffirmPage();
}

function clickDim() {

    g_sinfo.judgetime = getEffectiveTime();
    g_sinfo.selJudge = EnumTypeJudge.ETJDim;
    toAffirmPage();
}

function clickOblivious() {

    g_sinfo.judgetime = getEffectiveTime();
    g_sinfo.selJudge = EnumTypeJudge.ETJOblivious;
    toStudyPage();
}

function clickUnknown() {

    g_sinfo.judgetime = getEffectiveTime();
    g_sinfo.selJudge = EnumTypeJudge.ETJUnknown;
    toStudyPage();
}
//////////////////////////////////////////////////////////////////////////
function initainfo() {
    bwingshow = learnMgr.getConfigInfo("UI/bwingshow", true) == "false" ? false : true;
    bdmeanshow = learnMgr.getConfigInfo("UI/bdmeanshow", true) == "false" ? false : true;
    bemeanshow = learnMgr.getConfigInfo("UI/bemeanshow", true) == "false" ? false : true;
    bmcodeshow = learnMgr.getConfigInfo("UI/bmcodeshow", true) == "false" ? false : true;
    bthesaurusshow = learnMgr.getConfigInfo("UI/bthesaurusshow", true) == "false" ? false : true;
    bsyllableshow = learnMgr.getConfigInfo("UI/bsyllableshow", true) == "false" ? false : true;
    bsuppletionshow = learnMgr.getConfigInfo("UI/bsuppletionshow", true) == "false" ? false : true;
    bsensshow = learnMgr.getConfigInfo("UI/bsensshow", true) == "false" ? false : true;
    bgnameshow = learnMgr.getConfigInfo("UI/bgnameshow", true) == "false" ? false : true;


    $("#owimg").attr('checked', bwingshow);
    $("#odmean").attr('checked', bdmeanshow); 
    $("#oemean").attr('checked', bemeanshow);
    $("#omcode").attr('checked', bmcodeshow);
    $("#othesaurus").attr('checked', bthesaurusshow);
    $("#osyllable").attr('checked', bsyllableshow);
    $("#osuppletion").attr('checked', bsuppletionshow);
    $("#osens").attr('checked', bsensshow);
    $("#ogname").attr('checked', bgnameshow);
}
function clickowimg() {
    bwingshow = $("#owimg").attr('checked');
    learnMgr.setConfigInfo("UI/bwingshow", bwingshow);
    log("clickowimg");
    updateCurWordBaseInfo(cwordinfo);
}

function clickodmean() {
    bdmeanshow = $("#odmean").attr('checked');
    learnMgr.setConfigInfo("UI/bdmeanshow", bdmeanshow);
    log("clickodmean");
    updateCurWordBaseInfo(cwordinfo);
}

function clickoemean() {
    bemeanshow = $("#oemean").attr('checked');
    learnMgr.setConfigInfo("UI/bemeanshow", bemeanshow);
    log("clickoemean");
    updateCurWordBaseInfo(cwordinfo);
}

function clickomcode() {
    bmcodeshow = $("#omcode").attr('checked');
    learnMgr.setConfigInfo("UI/bmcodeshow", bmcodeshow);
    log("clickomcode");
    updateCurWordBaseInfo(cwordinfo);
}

function clickothesaurus() {
    bthesaurusshow = $("#othesaurus").attr('checked');
    learnMgr.setConfigInfo("UI/bthesaurusshow", bthesaurusshow);
    log("clickothesaurus");
    updateCurWordBaseInfo(cwordinfo);
}

function clickosyllable() {
    bsyllableshow = $("#osyllable").attr('checked');
    learnMgr.setConfigInfo("UI/bsyllableshow", bsyllableshow);
    log("clickosyllable");
    updateCurWordBaseInfo(cwordinfo);
}

function clickosuppletion() {
    bsuppletionshow = $("#osuppletion").attr('checked');
    learnMgr.setConfigInfo("UI/bsuppletionshow", bsuppletionshow);
    log("clickosuppletion");
    updateCurWordBaseInfo(cwordinfo);
}

function clickosens() {
    bsensshow = $("#osens").attr('checked');
    learnMgr.setConfigInfo("UI/bsensshow", bsensshow);
    log("clickosens");
    updateCurWordBaseInfo(cwordinfo);
}

function clickogname() {
    bgnameshow = $("#ogname").attr('checked');
    learnMgr.setConfigInfo("UI/bgnameshow", bgnameshow);
    log("clickogname");
    updateCurWordBaseInfo(cwordinfo);
}

function clickrespeak() {

    learnMgr.setLoopPlay($("#respeak").attr('checked'));


}
lsJudge = 1;
lsAffirm = 2;
lsStudy = 3;
var learnstate;
//////////////////////////////////////////////////////////////////////////
function toJudgePage() {

    learnstate = lsJudge;
    //log("toJudgePage " + nowUseSkill);
    $(".question").show();
    switch (nowUseSkill) {
        case "e_c":
            $(".question .lheight").show();
            $(".question .mean").hide();
            $(".question .wordspeak").show();
            $(".question .iword").hide();
            clickSpeak();
            break;
        case "c_e":
            $(".question .mean").show();
            $(".question .wordspeak").show();
            $(".question .iword").hide();
            $(".question .lheight").hide();            
            break;
        case "l_c":
            $(".question .word").hide();
            $(".question .pron").show();
            $(".question .mean").hide();
            $(".question .wordspeak").show();
            $(".question .lheight").show();            
            $(".question .iword").hide();
            clickSpeak();
            break;
        case "c_w":
            $("#iword").val("");
            //$("#iword").focus();
            $(".question .word").hide();
            $(".question .pron").hide();
            $(".question .mean").show();
            $(".question .wordspeak").hide();
            $(".question .iword").show();
            //clickSpeak();
            break;
        case "l_w":
            $("#iword").val("");
            //$("#iword").focus();
            $(".question .word").hide();
            $(".question .pron").hide();
            $(".question .mean").hide();
            $(".question .lheight").show();  
            $(".question .wordspeak").show();
            $(".question .iword").show();
            clickSpeak();
            break;
        case "p_w":
            $("#iword").val("");
            //$("#iword").focus();
            $(".question .word").hide();
            $(".question .pron").show();
            $(".question .mean").show();
            $(".question .lheight").show();  
            $(".question .wordspeak").show();
            $(".question .iword").show();
            clickSpeak();
            break;
        default:
            ;
    }
    
    $(".judge").show();
    $(".affirm").hide();
    $(".study").hide();

    //$(".question")

    $(".answer").hide();
    $(".ainfo").hide();
    $("#tab").hide();
    g_statestarttime = new Date().getTime();
    g_overtimeCount = 0;
    // learnMgr.logOut(g_sinfo.judgetime);
  
}

function toAffirmPage() {

    learnstate = lsAffirm;
    log("toAffirmPage " + nowUseSkill);
    switch (nowUseSkill) {
        case "e_c":
            $(".answer .word").hide();
            $(".answer .pron").hide();
            $(".answer .mean").show();
            $(".answer .wordspeak").hide();            
            break;
        case "c_e":
            $(".answer .word").show();
            $(".answer .pron").show();
            $(".answer .mean").hide();
            $(".answer .wordspeak").show();
            clickSpeak();
            break;
        case "l_c":
            $(".answer .word").hide();
            $(".answer .pron").hide();
            $(".answer .mean").show();
            $(".answer .wordspeak").hide();
            $(".question .word").show();
            clickSpeak();        
            break;
        default:
            ;
    }    
    $(".judge").hide();
    $(".affirm").show();
    $(".study").hide();

    $("#tab").show();
    $(".answer").show();
    $(".ainfo").show();
    g_statestarttime = new Date().getTime();
    g_overtimeCount = 0;

}

function toStudyPage() {
    log("toStudyPage 1");
    learnstate = lsStudy;
    switch (nowUseSkill) {
        case "e_c":
            $(".answer .word").hide();
            $(".answer .pron").hide();
            $(".answer .mean").show();
            $(".answer .wordspeak").hide();
            break;
        case "c_e":
            $(".answer .word").show();
            $(".answer .pron").show();
            $(".answer .mean").hide();
            $(".answer .wordspeak").show();
            
            break;
        case "l_c":
            $(".answer .word").hide();
            $(".answer .pron").hide();
            $(".answer .mean").show();
            $(".answer .wordspeak").hide();
            $(".question .word").show();

            break;
        case "c_w":
            $(".question .word").show();
            $(".question .pron").show();
            $(".question .mean").show();
            $(".question .wordspeak").show();
            $(".question .iword").show();
            $(".answer .word").hide();
            $(".answer .pron").hide();
            $(".answer .mean").hide();
            $(".answer .wordspeak").hide();
            break;
        case "l_w":
           $(".question .word").show();
            $(".question .pron").show();
            $(".question .mean").show();
            $(".question .wordspeak").show();
            $(".question .iword").show();
            $(".answer .word").hide();
            $(".answer .pron").hide();
            $(".answer .mean").hide();
            $(".answer .wordspeak").hide();        
            break;
        case "p_w":
           $(".question .word").show();
            $(".question .pron").show();
            $(".question .mean").show();
            $(".question .wordspeak").show();
            $(".question .iword").show();
            $(".answer .word").hide();
            $(".answer .pron").hide();
            $(".answer .mean").hide();
            $(".answer .wordspeak").hide();        
            break;            
        default:
            ;
    }  

    clickSpeak();
    
    $(".judge").hide();
    $(".affirm").hide();
    $(".study").show();

    $("#tab").show();
    $(".answer").show();
    $(".ainfo").show();
    g_statestarttime = new Date().getTime();
    g_overtimeCount = 0;
}

function updateCurBookInfo(params) {
    $("#bname").html(params.Name);
};

function updateCurWordBaseInfo(params) {

    try {
        $(".word").html(params.word);
        if (params.pron == "") {
            $(".pron").html(""); // 占位,否则行会跳动
        }
        else {
            $(".pron").html("/" + params.pron + "/");
        }
        $(".mean").html(params.mean.replace(/\n/g, "<br />"));

        // $(".dmean").html(params.dmean);

        if (!bdmeanshow || params.dmean == "") {
            $("#dmean").parent().hide();
        }
        else {
            $("#dmean").parent().show();
            $("#dmean").html(params.dmean.replace(/\n/g, "<br />"));
        }

        if (!bemeanshow || params.emean == "") {
            $("#emean").parent().hide();
        }
        else {
            $("#emean").parent().show();
            $("#emean").html(params.emean);
        }

        if (!bmcodeshow || params.mcode == "") {
            $("#mcode").parent().hide();
        }
        else {
            $("#mcode").parent().show();
            $("#mcode").html(params.mcode);
        }

        if (!bthesaurusshow || params.thesaurus == "") {
            $("#thesaurus").parent().hide();
        }
        else {
            $("#thesaurus").parent().show();
            $("#thesaurus").html(params.thesaurus);
        }

        if (!bsyllableshow || params.syllable == "") {
            $("#syllable").parent().hide();
        }
        else {
            $("#syllable").parent().show();
            $("#syllable").html(params.syllable);
        }

        if (!bsuppletionshow || params.suppletion == "") {
            $("#suppletion").parent().hide();
        }
        else {
            $("#suppletion").parent().show();
            $("#suppletion").html(params.suppletion);
        }
        if (!bsensshow || params.sens == "") {
            $("#sens").parent().hide();
        }
        else {
            $("#sens").parent().show();
            //alert(params.sens);
            html =  params.sens.replace(/\n\n/g, "").replace(/&speakico/g, '<img class="icon icon25" src="../../images/cleardot.gif" onclick="speakSens(this);" />') + "<br />";
            $("#sens").html(html);
        }
        //learnMgr.logOut(params.gname); 分组名
        if (!bgnameshow || params.gname == "") {
            $("#gname").parent().hide();
        }
        else {
            $("#gname").parent().show();
            $("#gname").html(params.gname);
        }

        if (!bwingshow || params.wimg == "") {
            //log("1");
            $("#wimg").parent().hide();
        }
        else {
            $("#wimg").parent().show();
            //log(2);
            $("#wimg").html('<img src="' + params.wimg + '" / >');
        }
    }
    catch (err) {
        log("updateCurWordBaseInfo err:" + err);
        $("#wimg").parent().hide();
        $("#dmean").parent().hide();
        $("#emean").parent().hide();
        $("#mcode").parent().hide();
        $("#thesaurus").parent().hide();
        $("#syllable").parent().hide();
        $("#suppletion").parent().hide();
        $("#sens").parent().hide();
        $("#gname").parent().hide();        
    }
    
};

function speakSens(myself) {
    var text = $(myself.previousSibling).html();
    log(text);
    learnMgr.speak(text);
}

function initTimer() {

    var timeout = 15000;

    $(document).bind("idle.idleTimer", function() {
        // alert($.idleTimer('getElapsedTime'));
        // learnMgr.logOut("idle" + $.idleTimer('getElapsedTime'));
        g_idlestarttime = new Date().getTime();
        learnMgr.logOut("空闲开始");
    });

    $(document).bind("active.idleTimer", function() {
        //alert($.idleTimer('getElapsedTime'));
        // learnMgr.logOut("active" + $.idleTimer('getElapsedTime'));
        g_overtimeCount += new Date().getTime() - g_idlestarttime;
        learnMgr.logOut("空闲结束,闲置时间共" + g_overtimeCount);
    });

    $.idleTimer(timeout);
}

function getEffectiveTime() {
    return new Date().getTime() - g_statestarttime - g_overtimeCount;
}

function updateJudgeInfo() {
}

function log(info) {
    learnMgr.logOut(info);
}

function clickgoback() {
    learnMgr.goback();
}

function updateSTime(tval) {
     //log(tval.stime);
     $("#stime").html(tval.stime); 
}



function updateSTATSInfo() {
    tval = learnMgr.getCSTATSInfo();
    $("#tscount").html(tval.tscount);
    $("#nscount").html(tval.nscount);
    $("#rvcount").html(tval.rvcount);
    $("#kncount").html(tval.kncount);
    $("#sspeed").html(tval.sspeed);
}

function updateDict(addr, dictid) {
    $(dictid + " iframe").load(function() {
        $("#dictloadding").hide();
        $(dictid + " iframe").show();
    });
    $(dictid + " iframe").hide();
    $("#dictloadding").show();

    log(addr.replace(/\[LIWord\]/i, cwordinfo.word));

    $(dictid + " iframe").attr("src", addr.replace(/\[LIWord\]/i, cwordinfo.word));

}

function idTabsHandle(id, list, set) {
    $("a", set).removeClass("selected").filter("[href='" + id + "']", set).addClass("selected");
    dictsel = id;
    //log(id);
//     if (dictsel == "#youdao")
//         $("#youdao iframe").attr("src", "http://dict.youdao.com/m/search?keyfrom=dict.mindex&vendor=%24vendor&q=" + cwordinfo.word);
//     else if (dictsel == "#dictcn")
    //         $("#dictcn iframe").attr("src", "http://dict.cn/mini.php?q=" + cwordinfo.word);
    log(dictsel + " iframe " + g_dictinfo.getAddrByID(dictsel));
    $("#dictloadding").hide();
    addr = g_dictinfo.getAddrByID(dictsel);
    if (addr) {
        updateDict(addr, dictsel);
//         $(dictsel + " iframe").hide();
//         $(dictsel + " iframe").attr("src", "waitiframe.html?url=" + addr.replace(/\[LIWord\]/i, cwordinfo.word));
        //         $(dictsel + " iframe").show();
//         $(dictsel + " iframe").load(function() {
//             $("#dictloadding").hide();
//             $(dictsel + " iframe").show();
//         });
//         $(dictsel + " iframe").hide();
//         $("#dictloadding").show();
//         
//         $(dictsel + " iframe").attr("src", addr.replace(/\[LIWord\]/i, cwordinfo.word));
    }
//     log(addr);
//    // $(dictsel + " iframe").attr("src", .replace(/\[LIWord\]/g, cwordinfo.word));
    for (i in list)
        $(list[i]).hide();
    $(id).show();
    return false;
}

//    console.log("123");
//     console.error("123");
//     console.warning("123");

// $(document).ready(function() {
//     curBookInfo.setName(learnMgr.getCurrentLearnBookInfo().Name);
//     setBName(curBookInfo.name);
// 
//     learnMgr.test.connect(slot);
// 
// });
// 
// function slot(params) {
//     setBName(params.Name);
// };
// 
// function setBName(bname) {
//     $("#bname").html(bname);
// };
// 
// curBookInfo = new BookInfo();
// 
// function BookInfo() {
//     this.name = "";
//     this.setName = setName;
// };
// 
// function setName(bname) {
//     this.name = bname;
// };


function DictInfo() {
    this.count = 0;
    this.diarray = new Array();

    this.setInfo = function(dInfo) {
        this.count = dInfo.count;
        for (i = 0; i < this.count; i++) {
            this.diarray[i] = new Array(dInfo[i + ",name"], dInfo[i + ",addr"]);
            this['#' + i] = dInfo[i + ",addr"];
        }
    }
    this.setIdTables = function() {
        for (i = 0; i < this.count; i++) {
            $("#tabdict ul").append('<li><a href="#' + i + '" class="regular">' + this.diarray[i][0] + '</a></li>');
            $("#tabdict").append('<div id="' + i + '" style="display: none;"><iframe src="about:blank;" width="100%" height="100%"></iframe></div>');
        }
    }
    this.getAddrByID = function(id) {
        return this[id];
    }
};

function setNowLM(lmname) {
    if (nowlm != lmname) {
        nowlm = lmname;
        lmInit();
        resetHotkey(keySet);
    }
    
    //log("222:" + lmname);
    learnMgr.setConfigInfo("LearnMgr/NowLM", lmname);
}

function getNowLM() {
    var sval = learnMgr.getConfigInfo("LearnMgr/NowLM", "lmsimple");
    //log("11111111:" + sval);
    return sval;
}

function resetUI(id) {

    //log("resetUI," + id);
    if ($("#" + AryTLl[Number(id)].Name).attr("checked") != true) {
        location.href = "../" + AryTLl[Number(id)].Path;
        learnMgr.setConfigInfo("UI/nowUI", AryTLl[Number(id)].ID)    
    }
    

    //location.href = "http://news.163.com";

}
function filllo_learnui() {

    //$("#lo_learnui").html();
    AryTLl = learnMgr.getThemeList();
    var htmls = "";
    for (i = 0; i < AryTLl.length; i++) {
        htmls += '<input type="radio" name="learnui" onclick="' + "resetUI('" + (i).toString() + "');" + '" id="' + AryTLl[i].ID + '" /><label for="' + AryTLl[i].ID + '" onclick="' + "resetUI('" + (i).toString() + "');" + '" title = "' + "作者:" + AryTLl[i].Author + '">' + AryTLl[i].Name + '</label><br />'
//         htmls += "名称:" + ary[i].Name + "<br />";
//         htmls += "作者:" + ary[i].Author + "<br />";
//         htmls += "详细:" + ary[i].Comment + "<br />";
//           log(ary[i].Name);
//          log(ary[i].Author);
//          log(ary[i].Comment);
    }
    $("#lo_learnui").html(htmls);


    //log("filllo_learnui" + learnMgr.getConfigInfo("UI/nowUI", "langisle"));
    $("#" + learnMgr.getConfigInfo("UI/nowUI", "li1")).attr("checked", true);
    
    
    log(learnMgr.getConfigInfo("UI/nowUI", "li1"));
    //$("#body").html(learnMgr.getFileData("/html/theme/langisle/learnmgr.html"));



}

function clickgwrandom() {
    learnMgr.setGWRandom($("#gwrandom").attr('checked'));
}

function clickshowShortcutKeyTip() {
    bshowShortcutKeyTip =  $("#showShortcutKeyTip").attr('checked');
    learnMgr.setConfigInfoBool("lo_other", bshowShortcutKeyTip);
    disableHotkeyTitle(keySet);
}


function setHotScheme(sname) {
    
    $('.keyTip').attr('disabled', 'true');
   
    if (sname == 'wesdxc') {
        keySet.ETJKnowwell = 'W';
        keySet.ETJPlain = 'E';
        keySet.ETJDim = 'S';
        keySet.ETJOblivious = 'D';
        keySet.ETJUnknown = 'C';
        keySet.ETAYes = 'E';
        keySet.ETANo = 'D';
        keySet.ETNext = 'E';
        keySet.ETReplay = 'X';
    }
    else if (sname == 'oilk.,') {
        keySet.ETJKnowwell = 'O';
        keySet.ETJPlain = 'I';
        keySet.ETJDim = 'L';
        keySet.ETJOblivious = 'K';
        keySet.ETJUnknown = ',';
        keySet.ETAYes = 'I';
        keySet.ETANo = 'K';
        keySet.ETNext = 'I';
        keySet.ETReplay = '.';    
    }
    else if (sname == 'arrows') {
        keySet.ETJKnowwell = 'up';
        keySet.ETJPlain = 'left';
        keySet.ETJDim = 'end';
        keySet.ETJOblivious = 'right';
        keySet.ETJUnknown = 'pagedown';
        keySet.ETAYes = 'left';
        keySet.ETANo = 'right';
        keySet.ETNext = 'left';
        keySet.ETReplay = 'down';
    }
    else if (sname == 'custom') {
        keySet.ETJKnowwell = learnMgr.getConfigInfo("LearnMgr/Hotkey/ETJKnowwell", 'W');
        keySet.ETJPlain = learnMgr.getConfigInfo("LearnMgr/Hotkey/ETJPlain", 'E');
        keySet.ETJDim = learnMgr.getConfigInfo("LearnMgr/Hotkey/ETJDim", 'S');
        keySet.ETJOblivious = learnMgr.getConfigInfo("LearnMgr/Hotkey/ETJOblivious", 'D');
        keySet.ETJUnknown = learnMgr.getConfigInfo("LearnMgr/Hotkey/ETJUnknown", 'E');
        keySet.ETAYes = learnMgr.getConfigInfo("LearnMgr/Hotkey/ETAYes", 'E');
        keySet.ETANo = learnMgr.getConfigInfo("LearnMgr/Hotkey/ETANo", 'D');
        keySet.ETNext = learnMgr.getConfigInfo("LearnMgr/Hotkey/ETNext", 'E');
        keySet.ETReplay = learnMgr.getConfigInfo("LearnMgr/Hotkey/ETReplay", 'X');
        $('.keyTip').attr('disabled', false);
        
    }    
    else {
        return;
    }

    oldkeySet = keySet;
    
    $("#ETJKnowwell").val(keySet.ETJKnowwell);
    $("#ETJPlain").val(keySet.ETJPlain);
    $("#ETJDim").val(keySet.ETJDim);
    $("#ETJOblivious").val(keySet.ETJOblivious);
    $("#ETJUnknown").val(keySet.ETJUnknown);
    $("#ETAYes").val(keySet.ETAYes);
    $("#ETANo").val(keySet.ETANo);
    $("#ETNext").val(keySet.ETNext);
    $("#ETReplay").val(keySet.ETReplay);
    //unbinkey();
    
    resetHotkey(keySet);
    //log(sname);
    learnMgr.setConfigInfo("LearnMgr/Hotkey", sname);
}


function replaytimesChange() {
    var checkValue = $("#replaytimes").val();

    learnMgr.setReplaytimes(checkValue);
}

function replaysleepChange() {
    var checkValue = $("#replaysleep").val();

    learnMgr.setReplaySleep(checkValue);
}

function clickfullscreen() {

    jkmegamenu.hidemenu1(0);
    learnMgr.fullScreen($("#fullScreen").attr("checked"));

}

function lookup(event) {
    
    if (event.which != 1) return;
    
    var query = window.getSelection().toString();
    if (query.length == 0) {
        currentSelWord = "";
        return;
    }
    query = query.replace(/^\s+|\s+$/g, "");
    if (currentSelWord == query) {
        return;
    }

    
    //alert(typeof (window.getSelection()) + "," + window.getSelection().anchorNode.tagName);
    
    if ($('#learnoption').is(":visible")) return; // 设置框不弹

    $("#ltextarea").html("");
    $("#addresult").html("");
    currentSelWord = query;
    var wordInfo = learnMgr.getWordInfo(query);

    $("#slbword").html(query);
    $("#slbpron").html(wordInfo["pron"] ? "/" + wordInfo["pron"] + "/" : "");
    $("#slbmean").html(wordInfo["dmean"] ? wordInfo["dmean"] : "");

    var doc = document.compatMode == "BackCompat" ? document.body : document.documentElement;        var win_s = (!!window.opera || navigator.userAgent.indexOf('AppleWebKit/') > -1) ? { x: window.innerWidth, y: window.innerHeight} : { x: doc.clientWidth, y: doc.clientHeight };
        var mou_s = { x: event.pageX || (event.clientX + doc.scrollLeft), y: event.pageY || (event.clientY + doc.scrollTop) };
        var mou_c = { x: event.clientX || event.pageX - window.pageXOffset, y: event.clientY || event.pageY - window.pageYOffset };
        $("#selwordbox")[0].style.left = mou_c.x + 20 + 280 > win_s.x ? mou_s.x - 20 - 280 : mou_s.x + 20;
        $("#selwordbox")[0].style.top = mou_c.y + 20 + 210 > win_s.y ? mou_s.y - 20 - 210 : mou_s.y + 20;

};

function stop(e) {
    e = e || window.event;
     e.stopPropagation();
    currentSelWord = "";
}
function giveup() {
    $("#selwordbox")[0].style.left = -2000;
    $("#selwordbox")[0].style.top = -2000;
}

function updatenewwordbooklist() {
    var bid = parseInt(learnMgr.getConfigInfo("LearnMgr/newwordbookbid", "0"));

    var listBook = learnMgr.getNewWordBookList();
    for (var i = 0; i < listBook.length; i++) {
        //alert(listBook[i]["bid"] + listBook[i]["name"]);
        var tt = $("<option value='" + listBook[i]["bid"] + "'>" + listBook[i]["name"] + "</option>");
        if (parseInt(listBook[i]["bid"]) == bid) {
            tt.attr("selected", true);
        }
        $("#newwordbooklist").append(tt); 
    }
}
function updatenewwordbooklistchange() {
    //log($("#newwordbooklist").val())
    learnMgr.setConfigInfo("LearnMgr/newwordbookbid", $("#newwordbooklist").val());
}
function playslbword() {
    learnMgr.speak($("#slbword").html());
}

function addnewword() {
    var bid = parseInt($("#newwordbooklist").val());
    var word = $("#slbword").text();
    var mean = $("#slbmean").val();
    //log(word + "," + mean);
    if (word.length > 0 && mean.length > 0) {
        if (learnMgr.addWordToBook(bid, word, mean))
        {
        	$("#addresult").html("添加成功.")
        }
        else {
        	$("#addresult").html("已经存在.")
        }
        	
    }
    else {
        $("#addresult").html("释义不能为空.")
    }


}

function updateLastWord() {

    if (cwordinfo) {
        $("#lwword").html(cwordinfo.word);
        if (cwordinfo.pron)
            $("#lwpron").html("/" + cwordinfo.pron + "/");
        else
            $("#lwpron").html("");
        
        $("#lwmean").html(cwordinfo.mean);    
    }


}

function keydwonHandler(event) {
     var ctl = $(event.target);
//     if (event.keyCode == 13 && ctl.attr("class") == 'iword') {
//          return;
//     }
    /*log(event);*/
    if ($('#learnoption').is(":hidden")) return;
    //alert($(this).parent().attr('tagName'));
    //alert($(event.target).attr("tagName"));
    
    if (ctl.attr("class") == "keyTip") {
        
        var code = event.which,        
                type = event.type,
                character = String.fromCharCode(code).toLowerCase(),
                special = special_keys[code],
                shift = event.shiftKey,
                ctrl = event.ctrlKey,
                alt = event.altKey,
                propagate = true, // default behaivour
                mapPoint = null;
                
        
        
        var modif = '';
        if (alt) modif += 'alt+';
        if (ctrl) modif += 'ctrl+';
        if (shift) modif += 'shift+';

        //unbinkey();
        //alert(e.keyCode + "," + e.ctrlKey + "," + e.shiftKey + "," + e.altKey);
        //log(event.keyCode + "," + event.ctrlKey + "," + event.shiftKey + "," + event.altKey);
        
        ctl.val("");
        var keyText;
        if (special) {
            keyText = modif + special;
            //ctl.val(keyText);
        }
        else if (character) {
            keyText = modif + character;
            if (ctrl || alt || shift) ctl.val(keyText);
        }
        
        log("keydwonHandler "+code + " " + keyText + " " + special);
        
        var cid = ctl.attr("id");
        learnMgr.setConfigInfo("LearnMgr/Hotkey/" + cid, keyText);


        keySet[cid] = keyText;

        
        //alert(oldkeySet[cid]);
        oldkeySet = keySet;
        
         resetHotkey(keySet);
        
        
        
        // event.stopPropagation ? event.stopPropagation() : (event.cancelBubble = true);
        
        //alert(modif + special + "," + modif + character);
        //alert(e.keyCode + "," + e.ctrlKey + "," + e.shiftKey + "," + e.altKey);
        //ctl.val();
     } 
 
     
}

function skillChange() {
    skillsel = $("#skill").val();
    learnMgr.setConfigInfo("LearnMgr/skill", skillsel);
}
    // 注意 jq1.4后的版本有问题,按一个键调用多次   
    //      lsJudge = 1;
    // lsAffirm = 2;
    // lsStudy = 3;
function iwordClick() {
    //alert("iwordClick");
    if (learnstate == lsJudge) {
        var word = $("#iword").val();
//         if (word.length == 0) {
//             $("#iwordstatus").html("单词不能为空!!");
//             return;
//         }

        if (word != cwordinfo.word) {
            $("#iwordstatus").html("<img style='height:16px;widht:16px' src='../../images/delete_32.png' />");

            // g_sinfo.selJudge = EnumTypeJudge.ETJOblivious;
            g_sinfo.selJudge = EnumTypeJudge.ETJPlain;
            g_sinfo.selAffirm = EnumTypeAffirm.ETANo;
            $("#iword").select();
            toStudyPage();
        }
        else {
            $("#iwordstatus").html("<img height='16px;' src='../../images/tick_32.png'/>");
            window.setTimeout(function(){
              $("#iwordstatus").html("");
            }, 800);
            
            g_sinfo.selJudge = EnumTypeJudge.ETJPlain;
            g_sinfo.selAffirm = EnumTypeAffirm.ETAYes;
            clickSNext();
        }
        
    }
    else if (learnstate == lsStudy) {
        $("#iwordstatus").html("");
        clickSNext();
    }
}

function gWordOrderChange() {
    var gWordOrder = $("#gWordOrder").val();
    learnMgr.setConfigInfo("LearnMgr/gWordOrder", gWordOrder);
    learnMgr.setOption("gWordOrder", gWordOrder);
}

function rvWordOrderChange() {
    var rvWordOrder = $("#rvWordOrder").val();
    learnMgr.setConfigInfo("LearnMgr/rvWordOrder", rvWordOrder);
    learnMgr.setOption("rvWordOrder", rvWordOrder);
}
