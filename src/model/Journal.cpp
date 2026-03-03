/*******************************************************
 Copyright (C) 2024 George Ef (george.a.ef@gmail.com)

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ********************************************************/

#include "Journal.h"

TrxData Journal::execute_bill(const SchedData& sched_d, wxString date)
{
    TrxData trx_d;
    trx_d.TRANSID           = 0;
    trx_d.ACCOUNTID         = sched_d.ACCOUNTID;
    trx_d.TOACCOUNTID       = sched_d.TOACCOUNTID;
    trx_d.PAYEEID           = sched_d.PAYEEID;
    trx_d.TRANSCODE         = sched_d.TRANSCODE;
    trx_d.TRANSAMOUNT       = sched_d.TRANSAMOUNT;
    trx_d.STATUS            = sched_d.STATUS;
    trx_d.TRANSACTIONNUMBER = sched_d.TRANSACTIONNUMBER;
    trx_d.NOTES             = sched_d.NOTES;
    trx_d.CATEGID           = sched_d.CATEGID;
    trx_d.TRANSDATE         = date;
    trx_d.FOLLOWUPID        = sched_d.FOLLOWUPID;
    trx_d.TOTRANSAMOUNT     = sched_d.TOTRANSAMOUNT;
    trx_d.COLOR             = sched_d.COLOR;
    return trx_d;
}

TrxModel::Full_Data Journal::execute_bill_full(const SchedData& sched_d, wxString date)
{
    TrxModel::Full_Data trx_xd;
    trx_xd.TRANSID           = 0;
    trx_xd.ACCOUNTID         = sched_d.ACCOUNTID;
    trx_xd.TOACCOUNTID       = sched_d.TOACCOUNTID;
    trx_xd.PAYEEID           = sched_d.PAYEEID;
    trx_xd.TRANSCODE         = sched_d.TRANSCODE;
    trx_xd.TRANSAMOUNT       = sched_d.TRANSAMOUNT;
    trx_xd.STATUS            = sched_d.STATUS;
    trx_xd.TRANSACTIONNUMBER = sched_d.TRANSACTIONNUMBER;
    trx_xd.NOTES             = sched_d.NOTES;
    trx_xd.CATEGID           = sched_d.CATEGID;
    trx_xd.TRANSDATE         = date;
    trx_xd.FOLLOWUPID        = sched_d.FOLLOWUPID;
    trx_xd.TOTRANSAMOUNT     = sched_d.TOTRANSAMOUNT;
    trx_xd.COLOR             = sched_d.COLOR;
    return trx_xd;
}

TrxSplitModel::DataA Journal::execute_splits(const SchedSplitDataA& qp_a)
{
    TrxSplitModel::DataA tp_a;
    for (auto& qp_d : qp_a) {
        TrxSplitData tp_d;
        // FIXME: tp_d.m_id is invalid
        tp_d.m_id            = qp_d.m_id;
        tp_d.m_trx_id_p      = 0;
        tp_d.m_category_id_p = qp_d.m_category_id_p;
        tp_d.m_amount        = qp_d.m_amount;
        tp_d.m_notes         = qp_d.m_notes;
        tp_a.push_back(tp_d);
    }
    return tp_a;
}

Journal::Data::Data()
    : TrxData(), m_bdid(0), m_repeat_num(0)
{
}

Journal::Data::Data(const TrxData& trx_d)
    : TrxData(trx_d), m_bdid(0), m_repeat_num(0)
{
}

Journal::Data::Data(const SchedData& sched_d)
    : Data(sched_d, sched_d.TRANSDATE, 1)
{
}

Journal::Data::Data(const SchedData& sched_d, wxString date, int repeat_num)
    : TrxData(execute_bill(sched_d, date)), m_bdid(sched_d.BDID), m_repeat_num(repeat_num)
{
    if (m_repeat_num < 1) {
        wxFAIL;
    }
}

Journal::Data::~Data()
{
}

Journal::Full_Data::Full_Data(const TrxData& trx_d)
    : TrxModel::Full_Data(trx_d), m_bdid(0), m_repeat_num(0)
{
}

Journal::Full_Data::Full_Data(const TrxData& trx_d,
    const std::map<int64 /* TRANSID */, TrxSplitDataA>& splits,
    const std::map<int64 /* TRANSID */, TagLinkDataA>& tags)
:
    TrxModel::Full_Data(trx_d, splits, tags), m_bdid(0), m_repeat_num(0)
{
}

Journal::Full_Data::Full_Data(const SchedData& sched_d)
    : Full_Data(sched_d, sched_d.TRANSDATE, 1)
{
}

Journal::Full_Data::Full_Data(const SchedData& sched_d,
    wxString date, int repeat_num)
:
    TrxModel::Full_Data(execute_bill_full(sched_d, date), {}, {}),
    m_bdid(sched_d.BDID), m_repeat_num(repeat_num)
{
    if (m_repeat_num < 1) {
        wxFAIL;
    }

    m_splits = execute_splits(SchedModel::split(sched_d));

    m_tags = SchedModel::taglink(sched_d);

    TrxModel::Full_Data::fill_data();
    displayID = wxString("");
}

Journal::Full_Data::Full_Data(const SchedData& sched_d,
    wxString date, int repeat_num,
    const std::map<int64 /* BDID */, SchedSplitDataA>& budgetsplits,
    const std::map<int64 /* BDID */, TagLinkDataA>& tags)
:
    TrxModel::Full_Data(execute_bill_full(sched_d, date), {}, {}),
    m_bdid(sched_d.BDID), m_repeat_num(repeat_num)
{
    if (m_repeat_num < 1) {
        wxFAIL;
    }

    const auto budgetsplits_it = budgetsplits.find(m_bdid);
    if (budgetsplits_it != budgetsplits.end()) {
        m_splits = execute_splits(budgetsplits_it->second);
    }

    const auto tag_it = tags.find(m_bdid);
    if (tag_it != tags.end()) m_tags = tag_it->second;

    TrxModel::Full_Data::fill_data();
    displayID = wxString("");
}

Journal::Full_Data::~Full_Data()
{
}


void Journal::setEmptyData(Journal::Data &data, int64 accountID)
{
    TrxModel::setEmptyData(data, accountID);
    data.m_bdid = 0;
    data.m_repeat_num = 0;
}

bool Journal::setJournalData(Journal::Data &data, Journal::IdB journal_id)
{
    if (!journal_id.second) {
        const TrxData *trx_n = TrxModel::instance().get_id_data_n(journal_id.first);
        if (!trx_n)
            return false;
        data.m_repeat_num = 0;
        data.m_bdid = 0;
        data.TRANSID           = trx_n->TRANSID;
        data.ACCOUNTID         = trx_n->ACCOUNTID;
        data.TOACCOUNTID       = trx_n->TOACCOUNTID;
        data.PAYEEID           = trx_n->PAYEEID;
        data.TRANSCODE         = trx_n->TRANSCODE;
        data.TRANSAMOUNT       = trx_n->TRANSAMOUNT;
        data.STATUS            = trx_n->STATUS;
        data.TRANSACTIONNUMBER = trx_n->TRANSACTIONNUMBER;
        data.NOTES             = trx_n->NOTES;
        data.CATEGID           = trx_n->CATEGID;
        data.TRANSDATE         = trx_n->TRANSDATE;
        data.LASTUPDATEDTIME   = trx_n->LASTUPDATEDTIME;
        data.DELETEDTIME       = trx_n->DELETEDTIME;
        data.FOLLOWUPID        = trx_n->FOLLOWUPID;
        data.TOTRANSAMOUNT     = trx_n->TOTRANSAMOUNT;
        data.COLOR             = trx_n->COLOR;
    }
    else {
        const SchedData *sched_n = SchedModel::instance().get_id_data_n(journal_id.first);
        if (!sched_n)
            return false;
        data.m_repeat_num = 1;
        data.TRANSID = 0;
        data.m_bdid            = sched_n->BDID;
        data.ACCOUNTID         = sched_n->ACCOUNTID;
        data.TOACCOUNTID       = sched_n->TOACCOUNTID;
        data.PAYEEID           = sched_n->PAYEEID;
        data.TRANSCODE         = sched_n->TRANSCODE;
        data.TRANSAMOUNT       = sched_n->TRANSAMOUNT;
        data.STATUS            = sched_n->STATUS;
        data.TRANSACTIONNUMBER = sched_n->TRANSACTIONNUMBER;
        data.NOTES             = sched_n->NOTES;
        data.CATEGID           = sched_n->CATEGID;
        data.TRANSDATE         = sched_n->TRANSDATE;
        data.LASTUPDATEDTIME   = "";
        data.DELETEDTIME       = "";
        data.FOLLOWUPID        = sched_n->FOLLOWUPID;
        data.TOTRANSAMOUNT     = sched_n->TOTRANSAMOUNT;
        data.COLOR             = sched_n->COLOR;
    }
    return true;
}

const TrxSplitModel::DataA Journal::split(Journal::Data& journal_d)
{
    return (journal_d.m_repeat_num == 0)
        ? TrxSplitModel::instance().find(
            TrxSplitCol::TRANSID(journal_d.TRANSID)
        ) : Journal::execute_splits(SchedSplitModel::instance().find(
            SchedSplitCol::TRANSID(journal_d.m_bdid)
        ));
}
