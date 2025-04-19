import logging
from telegram import Update
from telegram.ext import ApplicationBuilder, CommandHandler, MessageHandler, filters, ContextTypes
import os
from dotenv import load_dotenv
load_dotenv()
logging.basicConfig(
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    level=logging.INFO
)

# Token do seu bot (substitua pelo seu token real ou use uma variável de ambiente)
BOT_TOKEN = os.getenv('BOT_TOKEN')

# Comando /start
async def start(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text(
        f"Olá, {update.effective_user.first_name}! Sou o Buszer, e eu te aviso quando o ônibus estiver chegando!🚌\n\n"
        "Comandos disponíveis:\n"
        "/ajuda - Mostra ajuda\n"
        "/info - Informações sobre o bot\n"
        "/horarios - Exibe horários do ônibus\n"
    )

# Comando /help
async def ajuda(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text(
        "Aqui estão os comandos disponíveis:\n"
        "/ajuda - Mostrar esta mensagem de ajuda\n"
        "/info - Informações sobre o bot\n"
        "/horarios - Exibe horários do ônibus\n"
    )

# Comando /info
async def info(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text(
        "Oi! Sou um Bot criado por alunos de engenharia da computação, e estou aqui para ajudar a comunidade do Centro de Informática. Comigo aqui, ninguém mais vai perder o ônibus circular!!!🚌🚌🚌"
    )
# Comando /horarios
async def horarios(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text(
        "Aqui estão os horários previstos do ônibus circular:" \
        "07h20 - Saída do CCHLA (sede) para o CI/CTDR\n"
        "08h00 - Saída do CI/CTDR para o CCHLA (sede)\n"
        "12h20 - Saída do CCHLA (sede) para o CI/CTDR\n"
        "13h00 - Saída do CI/CTDR para o CCHLA (sede)\n"
        "18h20 - Saída do CCHLA (sede) para o CI/CTDR\n"
        "19h00 - Saída do CI/CTDR para o CCHLA (sede)\n"
        "21h30 - Saída do CCHLA (sede) para o CI/CTDR\n"
        "22h00 - Saída do CI/CTDR para o CCHLA (sede)\n"  
    )


# Novo comando /chatid
async def chatid(update: Update, context: ContextTypes.DEFAULT_TYPE):
    chat_id = update.effective_chat.id
    chat_type = update.effective_chat.type
    chat_title = update.effective_chat.title if update.effective_chat.title else "Chat privado"
    
    await update.message.reply_text(
        f"Informações do chat:\n"
        f"ID: {chat_id}\n"
        f"Tipo: {chat_type}\n"
        f"Nome: {chat_title}\n\n"
        "Use este ID no código da ESP8266 para enviar mensagens para este chat."
    )

# Responder a mensagens normais (não comandos)
async def text_message(update: Update, context: ContextTypes.DEFAULT_TYPE):
    chat_id = update.effective_chat.id
    print(f"Mensagem recebida no chat ID: {chat_id}")
    
    await update.message.reply_text(
        f"Recebi sua mensagem: '{update.message.text}'\n"
        "Use /help para ver os comandos disponíveis."
    )

# Função principal
def main():
    # Criar a aplicação
    application = ApplicationBuilder().token(BOT_TOKEN).build()
    
    # Adicionar handlers de comandos
    application.add_handler(CommandHandler("start", start))
    application.add_handler(CommandHandler("help", ajuda))
    application.add_handler(CommandHandler("info", info))
    application.add_handler(CommandHandler("echo", horarios))
    application.add_handler(CommandHandler("chatid", chatid))
    
    # Handler para mensagens de texto normais
    application.add_handler(MessageHandler(filters.TEXT & ~filters.COMMAND, text_message))
    
    # Iniciar o bot
    print("Bot iniciado! Pressione Ctrl+C para parar.")
    application.run_polling()


if __name__ == "__main__":
    main()