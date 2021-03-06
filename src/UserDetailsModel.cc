/*  This file is part of WP4 (http://github.com/oscarsaleta/WP4)
 *
 *  Copyright (C) 2016  O. Saleta
 *
 *  WP4 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "UserDetailsModel.h"
#include "Session.h"
#include "User.h"

UserDetailsModel::UserDetailsModel(Session &session, Wt::WObject *parent)
    : Wt::WFormModel(parent), session_(session)
{
}

void UserDetailsModel::save(const Wt::Auth::User &authUser)
{
    Wt::Dbo::ptr<User> user = session_.user(authUser);
}